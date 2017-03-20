#pragma once
#include "cinder/app/App.h"
#include "TextureView.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class TextureWall : public View
{
private:

	typedef vector<TextureView> TextureRow;
	typedef vector<TextureRow> TextureGrid;
	TextureGrid mGrid;

public:

	TextureWall() = default;

	TextureWall(uvec2 gridSize)
	{
		setGridSize(gridSize);
	}

	void setGridSize(uvec2 gridSize)
	{
		if (gridSize.x && gridSize.y){
			mGrid.resize(gridSize.y);
			std::for_each(mGrid.begin(), mGrid.end(), [&](TextureRow& row)
			{
				row.resize(gridSize.x);
			});
		}
	}

	uvec2 getGridSize()
	{
		if (!mGrid.empty()){
			TextureRow& row = mGrid.front();
			if (!row.empty()){
				return uvec2(row.size(), mGrid.size());
			}
		}
		return uvec2();
	}

	TextureView* getTextureView(uvec2 position)
	{
		uvec2 gridSize = getGridSize();

		if (position.x > gridSize.x || position.y > gridSize.y)
			throw new std::out_of_range("Out of range");

		return &mGrid.at(position.y).at(position.x);
	}

	void draw()
	{
		if (mGrid.empty()){
			return;
		}

		Rectf celBounds = Rectf(vec2(), vec2(gl::getViewport().second) / vec2(getGridSize()));
		uvec2 nextGridPosition;

		for (auto rowIt = mGrid.begin(); rowIt != mGrid.end(); rowIt++)
		{
			nextGridPosition.y = std::distance(mGrid.begin(), rowIt);
			TextureRow& row = *rowIt;

			for (auto colIt = row.begin(); colIt != row.end(); colIt++)
			{
				nextGridPosition.x = std::distance(row.begin(), colIt);
				TextureView& textureView = *colIt;
				textureView.setBounds(celBounds.getOffset(celBounds.getSize() * vec2(nextGridPosition)));
				textureView.draw();
			}
		}
	}
};
