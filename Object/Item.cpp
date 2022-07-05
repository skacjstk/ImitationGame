#include "ImitationGame/framework.h"
#include "Item.h"

Item::~Item()
{
	SAFE_DELETE(frontImage_);
}

void Item::FrontImageUpdate(Matrix V, Matrix P)
{
//	if (frontImage_ != nullptr)
	frontImage_->SetPosition(GetPosition());
	frontImage_->SetRotation(0.0f, 0.0f, 0.0f);
	frontImage_->SetScale(GetScale());
	frontImage_->Update(V, P);
}

void Item::FrontImageRender()
{
//	if (frontImage_ != nullptr)
	frontImage_->Render();
}
