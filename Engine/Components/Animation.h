#pragma once

class SceneObject;

struct Keyframe
{
	int Frame = 0;

	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 Scale = glm::vec3(1.0f);
	glm::vec3 Rotation = glm::vec3(0.0f);
};

enum class AnimState
{
	Playing, Paused, Finished
};

class Animation
{
public:
	Animation(const std::string& name, SceneObject* sceneObj, const uint16_t& fps);
	~Animation();

	void Update();

	void StartAnimation();
	void PlayAnimation();
	void PauseAnimation();
	void ResetAnimation();
	
	void SetInterpolationFunc(const std::function<float(int, int)>& func);

	void SetStartKeyframe(Keyframe keyframe) { m_StartKeyframe = keyframe; }
	void InsertKeyframe(Keyframe* keyframe);
	void DeleteKeyframe(const int& index);
	Keyframe* GetKeyframe(const int& index);

	void DeleteTimeline();
	void PrintTimeline();
	
	
private:
	void Interpolate();
	bool HasNextKeyframe();
	Keyframe* GetNextKeyFrame();

	std::string m_Name;
	Keyframe m_StartKeyframe;
	std::vector<Keyframe*> m_Timeline;
	float m_AnimSequenceTime;
	std::function<float(float, float)> m_InterpolationFunc;

	AnimState m_State;

	uint16_t m_CurrentFrame;
	uint16_t m_Fps;

	int m_CurrentKeyFramePosition;
	Keyframe* m_CurrentKeyframe;
	Keyframe* m_LastKeyframe;

	SceneObject* m_SceneObject;

};
