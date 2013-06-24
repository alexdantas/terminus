bool GameObject::collidesWith(GameObject *other){
    float dx, dy, radiusSum;

    radiusSum = box.radius + other->box.radius;
    dx = (box.x + box.radius) - (other->box.x + other->box.radius);
    dy = (box.y + box.radius) - (other->box.y + other->box.radius);

    if(radiusSum * radiusSum >= dx * dx + dy * dy)
        return true;

    return false;
}