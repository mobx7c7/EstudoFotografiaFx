#pragma once
#include "cinder/gl/gl.h"
#include "View.h"

using namespace ci;
using namespace ci::app;
using namespace std;

typedef shared_ptr<class TextureView> TextureViewRef;

class TextureView : public View
{
private:
	weak_ptr<gl::Texture2d> mTexture;
	Rectf mBounds;
public:
	void setTexture(gl::Texture2dRef& texture){ mTexture = texture; }
	gl::Texture2dRef getTexture(){ mTexture.lock(); }
	void setBounds(Rectf bounds){ mBounds = bounds; }
	Rectf getBounds(){ return mBounds; }
	void draw()
	{
		gl::ScopedColor scpColor;

		gl::color(Color::hex(0x101010));
		gl::drawSolidRect(mBounds);

		if (!mTexture.expired())
		{
			gl::Texture2dRef tex = mTexture.lock();

			Rectf srcBounds = tex->getBounds();
			Rectf dstBounds = mBounds;

			if (srcBounds.getAspectRatio() < dstBounds.getAspectRatio())
				dstBounds.scaleCentered(vec2(srcBounds.getAspectRatio() / dstBounds.getAspectRatio(), 1.0f));
			else
				dstBounds.scaleCentered(vec2(1.0f, dstBounds.getAspectRatio() / srcBounds.getAspectRatio()));

			gl::color(Color::white());
			gl::draw(tex, dstBounds);
			
			gl::color(Color::hex(0x202020));
			gl::drawStrokedRect(dstBounds);

			gl::drawString(tex->getLabel(), ivec2(mBounds.getUpperLeft()));
		}
	
		gl::color(Color::hex(0x404040));
		gl::drawStrokedRect(mBounds);
	}
};