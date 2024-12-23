#include "GamEncin.h"
#include <iostream>

class AnyObject : public Object
{
public:
    void Awake() override
    {
		Object::Awake();
        //name = "myObject";
        position = Vector3::one;
        Layer layer = Default;
    }

    void Start() override
    {
		Object::Start();
        cout<< name;
    }

    void Update() override
    {
		Object::Update();
    }

    void FixUpdate() override
    {
		Object::FixUpdate();
    }
};