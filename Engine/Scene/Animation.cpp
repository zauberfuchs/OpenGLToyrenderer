#include "../../Engine/Utils/pch.h"
#include "Animation.h"

#include "SceneObject.h"

Animation::Animation(const std::string& name, SceneObject* sceneObj, const uint16_t& fps)
	: m_Name(name), m_CurrentFrame(0), m_Fps(fps), m_SceneObject(sceneObj), m_CurrentKeyFramePosition(0), m_State(AnimState::Finished)
{
	m_InterpolationFunc = [](const float& i, const float& frames) {
		return i / (frames - 1);
	};
	sceneObj->SetAnimation(this);
	m_StartKeyframe.Frame = 0;
	m_StartKeyframe.Position = m_SceneObject->GetTransform()->GetLocalPosition();
	m_StartKeyframe.Scale = m_SceneObject->GetTransform()->GetLocalScale();
	m_StartKeyframe.Rotation = m_SceneObject->GetTransform()->GetLocalEulerAngles();
	m_LastKeyframe = &m_StartKeyframe;
}

Animation::~Animation()
{
}

// interpolate the transform and count the frames + 1
void Animation::Update()
{
	if (m_State == AnimState::Playing)
	{
		Interpolate();
		m_CurrentFrame++;
	}
}

// start the Animation if it has atleast 2 keyframes and is finished else just unpause it
void Animation::StartAnimation()
{
	if(m_Timeline.size() >=2 && m_State == AnimState::Finished)
	{
		m_CurrentKeyframe = m_Timeline[0];
		PlayAnimation();
	}
	else if(m_State == AnimState::Paused)
	{
		PlayAnimation();
	}
}


void Animation::PlayAnimation()
{
	m_State = AnimState::Playing;
}

void Animation::PauseAnimation()
{
	m_State = AnimState::Paused;
}

// reset the animation state and all parameters
void Animation::ResetAnimation()
{
	m_CurrentFrame = 0;
	m_CurrentKeyFramePosition = 0;
	m_SceneObject->GetTransform()->Translate(m_StartKeyframe.Position);
	m_SceneObject->GetTransform()->Scale(m_StartKeyframe.Scale);
	m_SceneObject->GetTransform()->SetRotation(m_StartKeyframe.Rotation);
	

	m_State = AnimState::Finished;
}

// set the interpolation function
void Animation::SetInterpolationFunc(const std::function<float(int, int)>& func)
{
	m_InterpolationFunc = func;
}

//insert a keyframe and sort it again by frame in the Timeline
void Animation::InsertKeyframe(Keyframe* keyframe)
{
	m_Timeline.push_back(keyframe);
	std::sort(m_Timeline.begin(), m_Timeline.end(), [](Keyframe* a, Keyframe* b) {
		return a->Frame < b->Frame;
		});
}

void Animation::DeleteKeyframe(const int& index)
{
	m_Timeline.erase(m_Timeline.begin() + index);
}

Keyframe* Animation::GetKeyframe(const int& index)
{
	return m_Timeline.at(index);
}

void Animation::DeleteTimeline()
{
	m_Timeline.clear();
}

void Animation::PrintTimeline()
{
	uint16_t i = 0;
	std::cout << "Keyframe Timeline:" << std::endl;
	std::cout << "---------------------------------" << std::endl;
	for(auto keyframe : m_Timeline)
	{
		std::cout << "Keyframe " << i << " at Frame: " << keyframe->Frame << std::endl;
		i++;
	}
	std::cout << "---------------------------------" << std::endl;
}

// interpolate every transformation component, with the formula wa + (we - wa) * f(i / #frames-i)
void Animation::Interpolate()
{
	if (m_CurrentFrame < m_CurrentKeyframe->Frame)
	{
		float interpolation = m_InterpolationFunc(m_CurrentFrame - m_LastKeyframe->Frame, m_CurrentKeyframe->Frame - m_LastKeyframe->Frame);
		m_SceneObject->GetTransform()->Translate(m_LastKeyframe->Position + (m_CurrentKeyframe->Position - m_LastKeyframe->Position) * interpolation);
		m_SceneObject->GetTransform()->Scale(m_LastKeyframe->Scale + (m_CurrentKeyframe->Scale - m_LastKeyframe->Scale) * interpolation);
		m_SceneObject->GetTransform()->SetRotation(m_LastKeyframe->Rotation + (m_CurrentKeyframe->Rotation - m_LastKeyframe->Rotation) * interpolation);
	}
	else if (m_CurrentKeyFramePosition < m_Timeline.size() - 1)
	{
		m_LastKeyframe = m_CurrentKeyframe;
		m_CurrentKeyframe = m_Timeline[++m_CurrentKeyFramePosition];
	}
	else
	{
		m_State = AnimState::Finished;
	}
}
