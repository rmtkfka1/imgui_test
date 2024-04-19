#pragma once

class Mesh;
class Texture;
class Shader;
class Transform;

class GameObject
{

public:
	void Init();
	void Update();
	void Render();

	void SetMesh(shared_ptr<Mesh> mesh) { _mesh = mesh; }
	void SetTexture(shared_ptr<Texture> texture) { _texture = texture; }
	void SetShader(shared_ptr<Shader> shader) { _shader = shader; }

private:

	shared_ptr<Transform> _transform;
	shared_ptr<Mesh> _mesh;
	shared_ptr<Shader> _shader;
	shared_ptr<Texture> _texture;


};

