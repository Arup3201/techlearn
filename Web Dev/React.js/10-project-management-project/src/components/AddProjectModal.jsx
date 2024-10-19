import { useState } from "react";
import Field from "./Field.jsx";

const AddProjectModal = ({ onSave, onCancel }) => {
  const [title, setTitle] = useState();
  const [description, setDescription] = useState();
  const [dueDate, setDueDate] = useState();

  return (
    <div className="w-[35rem] mt-16">
      <menu className="flex items-center justify-end gap-4 my-4">
        <li>
          <button
            className="text-stone-800 hover:text-stone-950"
            onClick={() => onCancel()}
          >
            Cancel
          </button>
        </li>
        <li>
          <button
            className="bg-stone-800 text-stone-50 hover:bg-stone-950 px-6 py-2 rounded-md"
            onClick={() =>
              onSave({
                title: title,
                description: description,
                dueDate: dueDate,
              })
            }
          >
            Save
          </button>
        </li>
      </menu>
      <div>
        <Field label="Title" onChange={(e) => setTitle(e.target.value)} />
        <Field
          label="Description"
          textarea
          onChange={(e) => setDescription(e.target.value)}
        />
        <Field
          label="Due Date"
          type="date"
          onChange={(e) => setDueDate(e.target.value)}
        />
      </div>
    </div>
  );
};

export default AddProjectModal;
