#include "effect.h"

#include <algorithm>

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QOpenGLFramebufferObject>

#include "application.h"
#include "vectorutil.h"
#include "layer.h"
#include "util/rectangle.h"
#include "glbuffer.h"

a2img::Effect::Effect(Application* app, QString category, QString name)
	: app_(app),
	  package_(category),
	  name_(name)
{
	loadManifest();
}

a2img::Effect::~Effect()
{
	// TODO: FIX THIS
	for(unsigned int i = 0; i < stages_.size(); i++) delete stages_[i].shader;
}

QString a2img::Effect::package()
{
	return package_;
}

QString a2img::Effect::name()
{
	return name_;
}

QString a2img::Effect::category()
{
	if(category_ == "") {
		return "Uncategorised";
	}

	return category_;
}

QString a2img::Effect::displayName()
{
	if(displayName_ == "") {
		return name_;
	}

	return displayName_;
}

QString a2img::Effect::relativePath()
{
	return QString(EFFECTS_DIRECTORY "/") + package() + "/" + name();
}

std::vector<a2img::EffectProperty*> a2img::Effect::properties()
{
	return uniqueVectorToRaw<a2img::EffectProperty>(templateProperties_);
}

void a2img::Effect::compileShaders()
{
	auto checkError = [=](EffectStage& stage, QString source) {
		if(	stage.shader->log() != "" /* NVIDIA */ &&
			stage.shader->log().indexOf("No errors.") < 0 /* Intel */) {
			QString message = stage.shader->log() + "\n\n*** Problematic shader source code* **\n\n" + source;
			throw InvalidEffectException(message.toStdString().c_str());
		}
	};

	for(EffectStage& stage : stages_) {
		QString vertexSource = processSourceFile(stage.vertexPath, true);
		QString fragmentSource = processSourceFile(stage.fragmentPath, true);

		stage.shader = new QOpenGLShaderProgram;
		stage.shader->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexSource);
		checkError(stage, vertexSource);
		stage.shader->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentSource);
		checkError(stage, fragmentSource);
		stage.shader->bindAttributeLocation("a2_position", 0);
		stage.shader->link();
		checkError(stage, "(Linking stage)\n\n" + vertexSource + fragmentSource);
	}
}

GLuint a2img::Effect::draw(GlFunctions* gl, const Texture& in, QSize size, std::vector<EffectProperty*> properties)
{
	std::vector<std::unique_ptr<QOpenGLFramebufferObject>> fbos;

	for(size_t i = 0; i < stages_.size(); i++) {
		EffectStage& stage = stages_[i];

		QOpenGLFramebufferObject* fbo = new QOpenGLFramebufferObject(size);

		fbo->bind();
		stage.shader->bind();

		gl->glViewport(0, 0, size.width(), size.height());
		gl->glClearColor(0, 0, 0, 1);
		gl->glClear(GL_COLOR_BUFFER_BIT);

		// Samplers

		in.bind(gl, 0, TextureQuality::smooth);

		gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(stage.wrapMode));
		gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(stage.wrapMode));

		GLint inputSamplerLocation = stage.shader->uniformLocation("a2_inputSampler");
		gl->glUniform1i(inputSamplerLocation, 0);

		std::vector<int> samplers;

		for(std::unique_ptr<QOpenGLFramebufferObject>& stageFbo : fbos) {
			gl->glActiveTexture(GL_TEXTURE0 + samplers.size() + 1);
			gl->glBindTexture(GL_TEXTURE_2D, stageFbo->texture());


			gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(stage.wrapMode));
			gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(stage.wrapMode));

			samplers.push_back(samplers.size() + 1);
		}
		GLint stageSamplerLocation = stage.shader->uniformLocation("a2_stageSamplers");
		gl->glUniform1iv(stageSamplerLocation, samplers.size(), samplers.data());

		// Resolution

		GLint resolutionLocation = stage.shader->uniformLocation("a2_resolution");
		gl->glUniform2f(resolutionLocation, size.width(), size.height());

		// Properties

		for(EffectProperty* property : properties) {
			property->attach(gl, stage.shader);
		}

		// Draw

		DefaultVertexBuffer vertexBuffer(gl);
		gl->drawArrays(vertexBuffer);

		fbos.emplace_back(fbo);
	}

	return (fbos.end() - 1)->get()->takeTexture();
}

void a2img::Effect::loadManifest()
{
	// Compatibility score for the
	// manifest stored in 'root'.
	int bestCompat = -1;
	QJsonObject root;

	// Loop until every manifest file has been checked.
	for(int i = 0;; i++) {
		QFile manifestFile(relativePath() + "/manifest" + QString::number(i) + ".json");
		manifestFile.open(QIODevice::ReadOnly);
		if(!manifestFile.isOpen()) {
			break;
		}

		QJsonDocument manifestDocument = QJsonDocument::fromJson(manifestFile.readAll());
		if(!manifestDocument.isObject()) {
			throw InvalidEffectException("Failed to load manifest: Invalid JSON document.");
		}
		QJsonObject currentRoot = manifestDocument.object();

		QJsonValue apiVersion = currentRoot["apiVersion"];
		if(!apiVersion.isString()) {
			throw InvalidEffectException("Failed to load manifest: Invalid API version.");
		}

		QStringList apiVersionEntries = apiVersion.toString().split('|');
		for(QString entry : apiVersionEntries) {
			if(entry == "*") {
				if(bestCompat == -1) {
					bestCompat = 999999;
					root = currentRoot;
				}
				continue;
			}

			// Check if the current manifest is more
			// compatible than the most compatible
			// manifest found so far.
			int currentCompat = VersionNumber(entry).compareCompatibility(EFFECTS_API_VERSION);
			if(currentCompat != -1 && (currentCompat < bestCompat || bestCompat == -1)) {
				bestCompat = currentCompat;
				root = currentRoot;
			}
		}
	}

	if(bestCompat < 0) {
		throw InvalidEffectException("Failed to load manifest: No manifest with a valid API version.");
	}

	QJsonValue category = root["category"];
	category_ = category.toString("");

	QJsonValue displayName = root["displayName"];
	displayName_ = displayName.toString("");

	QJsonValue stages = root["stages"];
	if(!stages.isArray()) {
		throw InvalidEffectException("Failed to load manifest: Invalid stages array.");
	}
	for(QJsonValue stageVal : stages.toArray()) {
		if(!stageVal.isObject()) {
			throw InvalidEffectException("Failed to load manifest: Invalid stage object.");
		}
		loadStage(stageVal.toObject());
	}

	QJsonValue properties = root["properties"];
	if(!properties.isArray()) {
		throw InvalidEffectException("Failed to load manifest: Invalid properties array.");
	}
	for(QJsonValue propertyVal : properties.toArray()) {
		if(!propertyVal.isObject()) {
			throw InvalidEffectException("Failed to load manifest: Invalid property object.");
		}
		loadProperty(propertyVal.toObject());
	}
}

void a2img::Effect::loadStage(QJsonObject stageObject)
{
	if(!stageObject["vertexShader"].isString()) {
		throw InvalidEffectException("Failed to load manifest: Invalid vertex shader path.");
	}

	if(!stageObject["fragmentShader"].isString()) {
		throw InvalidEffectException("Failed to load manifest: Invalid fragment shader path.");
	}

	QString vertexPath = stageObject["vertexShader"].toString();
	QString fragmentPath = stageObject["fragmentShader"].toString();

	WrapMode wrapMode = WrapMode::repeat;

	QJsonValue wrapStr = stageObject["wrapMode"].toString("");
	if(wrapStr == "mirorredRepeat") {
		wrapMode = WrapMode::mirroredRepeat;
	} else if(wrapStr == "clampToEdge") {
		wrapMode = WrapMode::clampToEdge;
	} else if(wrapStr == "clampToBorder") {
		wrapMode = WrapMode::clampToBorder;
	}

	EffectStage stage;
	stage.vertexPath = relativePath() + "/" + vertexPath;
	stage.fragmentPath = relativePath() + "/" + fragmentPath;
	stage.wrapMode = wrapMode;
	stages_.push_back(stage);
}

void a2img::Effect::loadProperty(QJsonObject propertyObject)
{
	QString type = propertyObject["type"].toString();

	EffectProperty* property = nullptr;
	if(type == "int") {
		property = new EffectPropertyInt(app_, propertyObject);
	} else if(type == "float") {
		property = new EffectPropertyFloat(app_, propertyObject);
	} else if(type == "enum") {
		property = new EffectPropertyEnum(app_, propertyObject);
	}

	if(property == nullptr) {
		throw InvalidEffectException("Failed to load manifest: Invalid property type.");
	}

	templateProperties_.emplace_back(property);
}

QString a2img::Effect::processSourceFile(QString glslFilePath, bool isTopLevel)
{
	QFile glslFile(glslFilePath);
	glslFile.open(QIODevice::ReadOnly);

	if(!glslFile.isOpen()) {
		QString message = QString("Failed to load source file: ") + glslFilePath + " doesn't exist.";
		throw InvalidEffectException(message.toStdString().c_str());
	}

	QString source = processSource(glslFile.readAll());

	if(!isTopLevel) {
		source = insertIncludeGuard(glslFilePath, source);
	}

	return source;
}

QString a2img::Effect::processSource(QString glslSourceCode)
{
	const char* startTag = "#pragma include ";

	QStringList sections = glslSourceCode.split(startTag);
	QStringList output;

	bool startsWithInclude = sections[0].indexOf(startTag) == 0;
	if(!startsWithInclude) {
		output.push_back(sections[0]);
		sections.removeFirst();
	}

	for(QString section : sections) {
		QChar includeType = section[0];
		section = section.mid(1);

		int endIndex;
		if(includeType == '<') {
			endIndex = section.indexOf('>');
		} else if(includeType == '"') {
			endIndex = section.indexOf('"');
		} else {
			throw InvalidEffectException("Failed to parse shader source: Malformed include directive. (1)");
		}

		if(endIndex < 0) {
			throw InvalidEffectException("Failed to parse shader source: Malformed include directive. (2)");
		}

		QString path = section.mid(0, endIndex);

		// Output the contents of the included file.
		// Recursively processes any additional
		// includes in that file.
		if(includeType == '<') {
			output.push_back(processSourceFile(":/effects_include/" + path, false));
		} else if(includeType == '"') {
			output.push_back(processSourceFile(relativePath() + "/" + path, false));
		} else {
			throw InvalidEffectException("Failed to parse shader source: Malformed include directive. (3)");
		}

		// Output the rest of this section, after the include.
		output.push_back(section.mid(endIndex + 1));
	}

	return output.join("");
}

QString a2img::Effect::insertIncludeGuard(QString filePath, QString sourceCode)
{
	filePath = filePath.toUpper();

	// Mangle the file path.
	std::transform(
		filePath.begin(),
		filePath.end(),
		filePath.begin(),
		[](QChar c) {
			if(c < 'A' || c > 'Z') {
				return QChar('_');
			}
			return c;
		}
	);

	QString identifier = "A2__INCLUDEGUARD__" + filePath;

	return	"\n#ifndef " + identifier +
			"\n#define " + identifier + "\n" +
			sourceCode +
			"\n#endif\n";
}
