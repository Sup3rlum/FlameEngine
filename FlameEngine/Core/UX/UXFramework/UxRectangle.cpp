#include "UxRectangle.h"


bool UxRectangle::Contains(FVector2 point)
{
	return (point.x > Position.x && point.y > Position.y) &&
		(point.x < Position.x + Size.x && point.y < Position.y + Size.y);
}