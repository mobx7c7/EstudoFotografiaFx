#pragma once
#include <cinder\gl\gl.h>
#include <vector>
using std::vector;
using namespace ci;

typedef std::shared_ptr<class DeviceRect> DeviceRectRef;

class DeviceRect
{
private:

	vector<gl::VboRef> mBuf;
	vector<gl::VaoRef> mVao;

public:

	DeviceRect()
	{
		mBuf.resize(2);
		mVao.resize(1);
	}

	DeviceRect(Rectf r = Rectf(vec2(0.0f), vec2(1.0f)))
	{
		vector<float> vbuf{ r.x1, r.y1, r.x2, r.y1, r.x2, r.y2, r.x1, r.y2 };
		vector<unsigned> ibuf{ 0, 1, 3, 3, 1, 2 };
		mBuf.push_back(gl::Vbo::create(GL_ARRAY_BUFFER, vbuf));
		mBuf.push_back(gl::Vbo::create(GL_ELEMENT_ARRAY_BUFFER, ibuf));
		mVao.push_back(gl::Vao::create());

		gl::ScopedVao scopedVao(mVao.at(0));
		mBuf.at(0)->bind();
		mBuf.at(1)->bind();
		gl::enableVertexAttribArray(0);
		gl::vertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

public:

	gl::VaoRef getVao(){ return mVao.front(); }

	gl::VboRef getVertices(){ return mBuf.front(); }

	gl::VboRef getIndices(){ return mBuf.back(); }

};

