class SceneHierarchyPanel
{
public:
	SceneHierarchyPanel() = default;
	SceneHierarchyPanel(const Ref<Scene>& scene);

	Entity		GetSelectedEntity() const { return m_SelectionContext; }

	void		SetContext(const Ref<Scene>& scene);
	void		OnImGuiRender();
	void		SetSelectedEntity(Entity entity) { m_SelectionContext = entity; };

private:
	void		DrawEntityNode(Entity entity);
	void		DrawComponents(Entity entity);
	void		DrawAddComponent(Entity entity);

private:
	Ref<Scene>	m_Context;
	Entity		m_SelectionContext;
	};
