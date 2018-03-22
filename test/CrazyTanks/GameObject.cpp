#include "GameObject.h"

void GameObject::setObjectType(int type)
{
	_object_type = type;
}

int GameObject::getObjectType()
{
	return _object_type;
}
