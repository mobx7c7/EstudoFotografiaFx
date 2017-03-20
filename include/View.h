#pragma once
#include <memory>

typedef std::shared_ptr<class View> ViewRef;

class View
{
private:
	View* mParentView;
protected:
	View() : mParentView(nullptr){}
public:
	virtual ~View() = default;
	void setParentView(View* parentView){ mParentView = parentView; }
	View* getParentView(){ return mParentView; }
	virtual void draw() = 0;
};
