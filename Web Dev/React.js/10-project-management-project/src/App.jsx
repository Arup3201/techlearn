import { useState, useRef } from "react";
import { ProjectSection } from "./components/ProjectSection";
import { Sidebar } from "./components/Sidebar";
import AddProjectModal from "./components/AddProjectModal";
import NoProjectSelected from "./components/NoProjectSelected.jsx";

function App() {
  const [projects, setProjects] = useState([]);
  const [selectedProject, setSelectedProject] = useState();
  const [addProject, setAddProject] = useState(false);

  function handleAddProject() {
    setAddProject(true);
  }

  function handleFormSave(projectDetails) {
    if (
      projectDetails?.title ||
      projectDetails?.description ||
      projectDetails?.dueDate
    ) {
      setProjects((prevProjects) => [
        ...prevProjects,
        {
          ...projectDetails,
          tasks: [],
        },
      ]);
      setSelectedProject({
        ...projectDetails,
        tasks: [],
      });
    }
  }

  function handleFormCancel() {
    setAddProject(false);
  }

  function handleProjectClick(project) {
    setSelectedProject({ ...project });
  }

  function handleAddTask(task) {
    if (task.title) {
      setSelectedProject((details) => ({
        ...details,
        tasks: [...details.tasks, task],
      }));
    }
  }

  function handleRemoveTask(taskId) {
    setSelectedProject((details) => {
      const newDetails = { ...details };
      const index = details.tasks.indexOf((task) => task.id === taskId);
      newDetails.tasks.splice(index, 1);
      return newDetails;
    });
  }

  return (
    <main className="h-screen flex gap-8 my-8">
      <Sidebar
        projects={projects}
        onAdd={handleAddProject}
        onClick={handleProjectClick}
      />
      {!addProject &&
        (selectedProject ? (
          <ProjectSection
            project={selectedProject}
            onAdd={handleAddTask}
            onRemove={handleRemoveTask}
          />
        ) : (
          <NoProjectSelected onAdd={handleAddProject} />
        ))}

      {addProject && (
        <AddProjectModal onSave={handleFormSave} onCancel={handleFormCancel} />
      )}
    </main>
  );
}

export default App;
