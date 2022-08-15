#pragma once


constexpr glm::vec3 X_ACHSE(1, 0, 0);
constexpr glm::vec3 Y_ACHSE(0, 1, 0);
constexpr glm::vec3 Z_ACHSE(0, 0, 1);

enum class Space
{
	World, Local
};

enum class Inheritance
{
	Translation = 1, Scale = 2, Rotation = 3
};

class Transform
{
public:
	Transform();


	void Translate(glm::vec3 translation, Space relativeTo = Space::Local);
	void Rotate(glm::vec3 eulers, Space relativeTo = Space::Local);
	void Scale(glm::vec3 scale, Space relativeTo = Space::Local);

	void RotateAxis(glm::vec3 axis);

	void SetParent(Transform* p);
	glm::mat4 GetParentMatrix();

	void SetConstraints(const std::initializer_list<Inheritance>& inheritance);
	bool* GetConstraints();

	void SetTransformMatrix(glm::mat4 matrix);
	void SetRotation(glm::vec3 eulers, Space relativeTo = Space::Local);

	glm::mat4 GetTransformMatrix();
	glm::mat4 GetConstrainedTransform(const bool* constraints);

	//Todo componnent system überarbeiten(ich gebe hier referenzen zurück)
	glm::vec3& GetLocalEulerAngles();
	glm::vec3& GetLocalPosition();
	glm::vec3& GetLocalScale();

	glm::vec3 GetWorldEulerAngles();
	glm::vec3 GetWorldPosition();
	glm::vec3 GetWorldScale();

private:

	glm::vec3 m_WorldPosition;
	glm::vec3 m_WorldEulerAngles;
	glm::vec3 m_WorldScale;
	
	glm::vec3 m_LocalPosition;
	glm::vec3 m_LocalEulerAngles;
	glm::vec3 m_LocalScale;

	glm::mat4 m_Axis;
	glm::mat4 m_TransformationMatrix;

	bool m_TransforMatrixSet;

	Transform* m_Parent = nullptr;

	bool m_Constraints[3] = { false };

};