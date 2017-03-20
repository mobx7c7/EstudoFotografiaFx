#pragma once
#include <cinder\app\App.h>
#include "DeviceRect.h"
using namespace ci;

typedef std::shared_ptr<class TextureBuilder> TextureBuilderRef;

class TextureBuilder : public Noncopyable
{
private:

	DeviceRect mDeviceRect;
	gl::FboRef mFramebuffer;
	gl::GlslProgRef mProgram;

private:

	TextureBuilder(gl::FboRef fbo) 
		: mFramebuffer(fbo) 
		, mDeviceRect(Rectf(vec2(0),vec2(1)))
	{}

public:

	~TextureBuilder() = default;

	TextureBuilder& compile(DataSourceRef fragmentSource)
	{
		static std::string vertexSource = CI_GLSL(440,
		in vec2 coord;
		uniform mat4 mvp;
		void main(){gl_Position = mvp * vec4(coord, 0, 1);});

		mProgram = gl::GlslProg::create(gl::GlslProg::Format().vertex(vertexSource).fragment(fragmentSource));
		return *this;
	}

	gl::Texture2dRef buildTexture()
	{
		if (mProgram)
		{
			/*
			gl::GlslProgRef prg = mPrg.front();
			gl::VaoRef vao = mDeviceRect->getVao();
			gl::ScopedVao scpVao(vao);
			gl::ScopedGlslProg scpProg(prg);
			prg->uniform("mvp", glm::ortho(0, 1, 1, 0));
			prg->uniform("resolution", vec2(getWindowSize()));
			prg->uniform("position", vec3(0, 0, timer.at(1)*timer.at(1)));
			prg->uniform("color", vec3(1, 1, 1));
			gl::drawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			*/

			gl::ScopedVao scpVao(mDeviceRect.getVao());
			gl::ScopedGlslProg scpProg(mProgram);
			mProgram->uniform("mvp", glm::ortho(0, 1, 1, 0));
			mProgram->uniform("resolution", vec2(mFramebuffer->getSize()));
			gl::drawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			return gl::Texture2d::create(mFramebuffer->getColorTexture()->createSource());
		}
		return gl::Texture2dRef();
	}

	static TextureBuilderRef create(ivec2 resolution, gl::Texture2d::Format& format = gl::Texture2d::Format())
	{
		gl::Fbo::Format fboFormat;
		fboFormat.colorTexture(format);
		return std::make_shared<TextureBuilder>(gl::Fbo::create(resolution.x, resolution.y, fboFormat));
	}
};
