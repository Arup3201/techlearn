'use client';

const todoInputHtml = document.getElementById('todo-input') as HTMLInputElement;
const ulistHtml = document.getElementById('todo-list') as HTMLUListElement;

todoInputHtml.addEventListener('keydown', function(e) {
    if(e.key.toLowerCase() !== 'enter') return;

    const listHtml = `
    <li class="todo-item">
        <input type="checkbox" class="todo-checkbox">
        <label for="todo1" class="todo-text">${todoInputHtml.value}</label>
    </li>
    `;

    ulistHtml.innerHTML += listHtml;

    // clear input 
    todoInputHtml.value = '';
});


