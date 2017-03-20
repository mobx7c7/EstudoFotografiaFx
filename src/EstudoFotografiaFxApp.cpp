#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Utilities.h"
#include "DeviceRect.h"
#include "TextureWall.h"
#include <random>

using namespace ci;
using namespace ci::app;
using namespace std;

class EstudoFotografiaFxApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
	void resize() override;
	void fileDrop(FileDropEvent event) override;

	vector<gl::Texture2dRef> mTex;
	vector<gl::FboRef> mFbo;
	vector<gl::GlslProgRef> mPrg;
	vector<gl::VboRef> mVbo;
	shared_ptr<DeviceRect> mDeviceRect;

	vector<float> timer;

	TextureWall mTextureWall;
	//map<int, ViewRef> mViews;
};

/*
pair<size_t, size_t> FindScope(string::iterator& beg, string::iterator& end)
{
	string text(beg, end);

	pair<size_t, size_t> offset(0u, 0u);

	offset.first = text.find_first_of('{');
	offset.second = text.find_first_of('}');

	if (offset.first != string::npos && offset.second != string::npos)
	{
		if (offset.first < offset.second)
		{

		}
	}

	return offset;
}

vector<pair<size_t, size_t>> FindScopes(string txt)
{
	vector<pair<size_t, size_t>> result;

	auto beg = txt.begin();
	auto end = txt.end();

	FindScope(beg, end);
	
	return result;
}
*/
void EstudoFotografiaFxApp::setup()
{
	timer = vector<float>(2, 1.0f);
	{
		gl::Texture::Format texFormat;
		texFormat.mipmap(true);
		texFormat.label("Grafico de fundo");
		mTex.push_back(gl::Texture2d::create(loadImage(loadAsset("back.jpg")), texFormat));
		//mTex.back()->setTopDown(true);

		texFormat.label("Mascara");
		mTex.push_back(gl::Texture2d::create(loadImage(loadAsset("slit plate 4.png")), texFormat));
		//mTex.back()->setTopDown(true);
	}

	{
		ivec2 fboSize(640,360);
		gl::Texture2d::Format texFormat;
		gl::Fbo::Format fboFormat;

		texFormat.label("Saida");
		texFormat.internalFormat(GL_RGBA8);
		fboFormat.attachment(GL_COLOR_ATTACHMENT0, gl::Texture2d::create(fboSize.x, fboSize.y, texFormat));

		texFormat.label("Camera");
		texFormat.internalFormat(GL_RGBA32F);
		fboFormat.attachment(GL_COLOR_ATTACHMENT1, gl::Texture2d::create(fboSize.x, fboSize.y, texFormat));

		texFormat.label("Sensor");
		texFormat.internalFormat(GL_RGBA32F);
		fboFormat.attachment(GL_COLOR_ATTACHMENT2, gl::Texture2d::create(fboSize.x, fboSize.y, texFormat));

		mFbo.push_back(gl::Fbo::create(fboSize.x, fboSize.y, fboFormat));
		gl::FboRef fbo = mFbo.back();

		mTextureWall = TextureWall(uvec2(3u, 2u));
		mTextureWall.getTextureView(uvec2(0u, 0u))->setTexture(mTex.at(0));
		mTextureWall.getTextureView(uvec2(1u, 0u))->setTexture(mTex.at(1));
		mTextureWall.getTextureView(uvec2(0u, 1u))->setTexture(fbo->getTexture2d(GL_COLOR_ATTACHMENT1));
		mTextureWall.getTextureView(uvec2(1u, 1u))->setTexture(fbo->getTexture2d(GL_COLOR_ATTACHMENT2));
		mTextureWall.getTextureView(uvec2(2u, 1u))->setTexture(fbo->getTexture2d(GL_COLOR_ATTACHMENT0));
	}

	/*
	try
	{
		mDeviceRect = make_shared<DeviceRect>(Rectf(vec2(0.0f), vec2(1.0f)));
		gl::GlslProg::Format progFormat;
		progFormat.vertex(loadString(loadAsset("quad.shader")));
		progFormat.fragment(loadString(loadAsset("slit plate.shader")));
		mPrg.push_back(gl::GlslProg::create(progFormat));
	}
	catch (gl::GlslProgExc e)
	{
		console() << e.what() << endl;
	}
	*/

	/*
	string vsrc;
	gl::GlslProg::Format progFormat;
	progFormat.vertex(vsrc);
	progFormat.fragment(loadString(loadAsset("slit plate.glsl")));
	mPrg.push_back(gl::GlslProg::create(progFormat));
	
	string txt = loadString(loadAsset("teste.script"));
	FindScopes(txt);

	*/
}

void EstudoFotografiaFxApp::mouseDown(MouseEvent event)
{

}

void EstudoFotografiaFxApp::fileDrop(FileDropEvent event)
{

}

void EstudoFotografiaFxApp::resize()
{
	ivec2 size = getWindowSize();
	
	/*
	if (size.x && size.y)
	{
		if (mFbo.empty())
			mFbo.resize(1);

		gl::FboRef& fbo = mFbo.front();
		gl::Texture2d::Format texFormat;
		gl::Fbo::Format fboFormat;
		texFormat.internalFormat(GL_RGBA8);
		fboFormat.attachment(GL_COLOR_ATTACHMENT0, gl::Texture2d::create(size.x, size.y, texFormat));
		texFormat.internalFormat(GL_RGBA32F);
		fboFormat.attachment(GL_COLOR_ATTACHMENT1, gl::Texture2d::create(size.x, size.y, texFormat));
		fboFormat.attachment(GL_COLOR_ATTACHMENT2, gl::Texture2d::create(size.x, size.y, texFormat));
		fbo = gl::Fbo::create(size.x, size.y, fboFormat);

		mTextureWall = TextureWall(uvec2(3u, 2u));
		mTextureWall.getTextureView(uvec2(0u, 0u))->setTexture(mTex.at(0));
		mTextureWall.getTextureView(uvec2(1u, 0u))->setTexture(mTex.at(1));
		mTextureWall.getTextureView(uvec2(0u, 1u))->setTexture(fbo->getTexture2d(GL_COLOR_ATTACHMENT1));
		mTextureWall.getTextureView(uvec2(1u, 1u))->setTexture(fbo->getTexture2d(GL_COLOR_ATTACHMENT2));
		mTextureWall.getTextureView(uvec2(2u, 1u))->setTexture(fbo->getTexture2d(GL_COLOR_ATTACHMENT0));
	}
	*/
}

void EstudoFotografiaFxApp::update()
{
}

void EstudoFotografiaFxApp::draw()
{
	gl::FboRef fbo0 = mFbo.front();

	if (fbo0)
	{
		gl::ScopedFramebuffer scpFbo(fbo0);
		gl::ScopedViewport scpViewport(ivec2(), fbo0->getSize());
		gl::ScopedMatrices scpMatrices;
		gl::setMatricesWindow(fbo0->getSize());
		
		static std::default_random_engine re;
		static std::uniform_real_distribution<float> rd;

		float k1 = timer.at(0);
		k1 += 0.001f * rd(re);
		k1 += 0.0005f * sin(2.0*M_PI*timer.at(0)*120.0f);
		k1 = pow(k1, 2.0f);

		function<void()> drawBackPlate = [&]()
		{
			//TODO: calcular fator de brilho relativo 
			//a quantidade de amostras para desenhar o rastro de luz

			/*
			gl::ScopedColor scpColor(Color::gray(0.1f));
			gl::Texture2dRef tex0 = mTex.at(0);
			Rectf imgSrc = tex0->getBounds();
			imgSrc *= 4.0f;
			imgSrc -= imgSrc.getCenter();
			imgSrc.offset(-vec2(imgSrc.getWidth() / 4.0f, 0.0f));
			imgSrc.offset(vec2(k1, 0) * vec2(600.0f));
			gl::draw(tex0, imgSrc);
			*/
			
			gl::ScopedColor scpColor(Color::gray(0.1f));
			gl::Texture2dRef tex0 = mTex.at(1);
			Rectf imgSrc = tex0->getBounds();
			imgSrc -= imgSrc.getCenter();
			gl::drawSolidRect(imgSrc);
			
		};

		function<void()> drawFrontPlate = [&]()
		{
			gl::Texture2dRef tex0 = mTex.at(1);
			Rectf imgSrc = tex0->getBounds();
			imgSrc -= imgSrc.getCenter();
			gl::draw(tex0, imgSrc);
		};

		gl::drawBuffer(GL_COLOR_ATTACHMENT1);
		{
			ivec2 fboSize = fbo0->getSize();
			gl::ScopedMatrices scpMatrices;
			CameraPersp cam(fboSize.x, fboSize.y, 45.0f);
			cam.lookAt(vec3(), vec3(0, 0, 1));
			gl::setMatrices(cam);
			gl::translate(-vec3(0, -fboSize.y * 0, -fboSize.x * k1));
			gl::rotate(glm::radians(180.0f));
			gl::clear();
			
			//Análogo a fotolito
			/*
			drawBackPlate();
			gl::ScopedBlend scpBlend(GL_DST_COLOR, GL_ZERO);
			drawFrontPlate();
			*/

			//Análogo a aplicação de máscara em imagem
			drawFrontPlate();
			gl::ScopedBlend scpBlend(GL_ZERO, GL_SRC_COLOR);
			drawBackPlate();
			

			timer.at(0) += 0.001f;
			timer.at(1) += 0.001f;
		}
		gl::drawBuffer(GL_COLOR_ATTACHMENT2);
		{
			gl::ScopedBlendAdditive scpBlend;
			gl::draw(fbo0->getTexture2d(GL_COLOR_ATTACHMENT1));
		}
		gl::drawBuffer(GL_COLOR_ATTACHMENT0);
		{
			gl::clear();
			gl::ScopedColor scpColor(Color(0.05f, 0.15f, 1.00f));
			//gl::ScopedColor scpColor(Color(0.06f, 0.25f, 1.00f));
			//gl::ScopedColor scpColor(Color(1.0f, 0.55f, 0.05f));
			//gl::ScopedColor scpColor(Color(0.15f, 0.05f, 1.0f));
			//gl::ScopedColor scpColor(Color(0.2f, 1.0f, 0.3f));
			gl::draw(fbo0->getTexture2d(GL_COLOR_ATTACHMENT2));
		}
	}
	
	gl::ScopedViewport scpViewport(ivec2(), getWindowSize());
	gl::clear();
	mTextureWall.draw();
}

CINDER_APP(EstudoFotografiaFxApp, RendererGl(RendererGl::Options().msaa(4)), [&](App::Settings *settings)
{
	settings->setConsoleWindowEnabled();
	//settings->setFrameRate(15.0f);
	//settings->disableFrameRate();
	settings->setWindowSize(ivec2(1280,720));
});
