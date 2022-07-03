#include "ImitationGame/framework.h"
#include "Item.h"

Item::~Item()
{
	SAFE_DELETE(frontImage_);
}

void Item::FrontImageUpdate(Matrix V, Matrix P)
{
//	if (frontImage_ != nullptr)
	frontImage_->SetPosition(this->GetPosition());
	frontImage_->SetScale(this->GetScale());
	frontImage_->Update(V, P);
}

void Item::FrontImageRender()
{
//	if (frontImage_ != nullptr)
	frontImage_->Render();
}
