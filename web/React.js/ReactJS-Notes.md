# Notes on Reast.JS - From Basics

## React Essentials - Components, JSX, Props, State and More...

### React Project Setup

To get started with React and writing code for that, we need a react project. One of the quickest ways to get started with react code, is to open a new chrome tab and type `react.new` it will open a code sandbox tab where everything that you might to build a react project is already present, and I just have to start writing react code only.

If you want to create a local environment to write react code then you can use `vite` or `create-react-app`. Also, you need `node.js` installed on your computer to make everything work.

To create a new react project using vite, you need to write:

```
npm create vite@latest react-project
```

Then you need to run the following command to download all the packages mentioned in the project:

```
npm install
```

Finally, you can run the development server using:

```
npm run dev
```

It will run the server at port `5173`.

**NOTE: Why do you need a special project setup to write react code?**

> The reason is, react uses HTML code with Javascript which is also known as jsx. Jsx is not supported in normal .js file if we just use the conventional set up of writing an index.html and app.js and linking them together. Unfortunately, this will give us errors in the web browser. The reason is as mentioned for jsx not being supported in web browser. And that is why we need a special set up that can convert this jsx to simple js that the web browser understands. React projects use a build process to include the script tags in the html file automatically for you, so you will never have to add those files explicitly. It simply means the code that you write is the code that is executed in the browser. Your code is transformed behind the scenes before it is handed over to the browser. One reason for the build process is because the raw, unprocessed code won’t run on the browser and another reason is the code is not optimized for production (i.e. minified). Fortunately, react projects come with this build process without any custom tweaking or set up.

### Import and Exports

In vanilla Javascript, we can always make variables, functions, classes available to other .js files by exporting them. Then, we can use import statements to import the specific variables, functions or classes from other files.

Let’s look at how we can do it.
_index.html_

```html
<!DOCTYPE html>
<html>
  <head>
    <title>JavaScript Refresher</title>
    <link rel="stylesheet" href="assets/styles/main.css" />
    <meta charset="UTF-8" />
    <script src="assets/scripts/app.js" type="module"></script>
  </head>

  <body>
    <header>
      <img src="assets/images/js-logo-xs.png" alt="JavaScript logo" />
      <h1>JavaScript Refresher</h1>
    </header>

    <ul>
      <li>Base Syntax & Rules</li>
      <li>Variables, Values & Operators</li>
      <li>Functions</li>
      <li>Objects</li>
      <li>Arrays</li>
      <li>Control Structures</li>
      <li>Browser APIs & The DOM</li>
      <li>Essential Features Used By React</li>
      <li>Tricky Parts</li>
    </ul>
  </body>
</html>
```

_assets/scripts/util.js_

```js
export let apiKey = "12345";
```

_assets/scripts/app.js_

```js
import { apiKey } from "./util.js";
console.log(apiKey);
```

It is necessary to write `type="module"` for any script where imports are used. Although, in react it is not needed for the same reason as react-script takes care of all modules by making a big file or set of big files that has all code included in it.

You just have to use export key to export the value and import syntax to import the particular value from the module. When mentioning the module name, you must specify its relative path. In vanilla JS, you must use .js but in react it is not mandatory.

Default export: Here you specify the default keyword after export to say that the value is the only value that will be exported from the file and there should no other default export. In default export, you should not use any let/const/var and the name of the variable.

_assets/scripts/util.js_

```js
export default "12345";
```

_assets/scripts/app.js_

```js
import apiKey from "./util.js";
console.log(apiKey);
```

When using default to export the value, you can use any name to assign that value when importing in another file.

`Alias`: When importing variables or functions from one file to another, you can use alias to store all imported variables in a javascript object and access the values using this object.
_assets/scripts/util.js_

```js
export default "abs";
export let apiKey = "123";
export let abc = 1;
```

_assets/scripts/app.js_

```js
import * as util from "./util.js";
console.log(util.apiKey, util.abc, util.default);
```

The default export is saved in the default key. Other exports has their key name same as their variable name.

### Components

Components are the reusable building blocks that make up a website. React apps are built by combining components.

Components are built using HTML, CSS and JS logic that you as a developer can potentially reuse throughout the website.

**Why use components?**

1. Reusable building blocks: Create small building blocks and compose the UI from them. If needed, reuse a building block in different part of the UI (e.g. a reusable button).
2. Related code lives together: Related HTML, JS (and possibly CSS) code is stored together. Since JS influences the output, Storing JS+HTML makes more sense.
3. Seperation of concern: Since different blocks handle different data and logic, this vastly simplfies the process of working on complex apps.

Component is a very important concept that is also used in other popular frameworks like vue, angular, flutter etc.

Download the project zip file: [Download](https://github.com/academind/react-complete-guide-course-resources/blob/main/attachments/03%20React%20Essentials/01-starting-project.zip).

Run the following commands:

```
npm install
npm run dev
```

You will see the project output. This is the starting point of the project.

When you open the `index.html` file, you will see almost nothing there. Apart from a strange looking `/src/index.jsx` source file.

```html
<!-- Code... -->
<script type="module" src="/src/index.jsx"></script>
<!-- Code... -->
```

When you visit this file, you will again not see a lot of code, but this code imports some stuff from `App.jsx` file.

```js
// Code...
import App from "./App.jsx";
// Code...
```

Now, when you go look at `App.jsx`, you will see some code that is generating the project output. Though there is some html code inside of javascript. This is `JSX` which stands for `Javscript Syntax eXtenstion`.

It is used to describe and create HTML elements in a declarative way.

With React you will write declarative code, which simply means you define the target HTML structure & UI - not the steps to get there.

But, JSX is not supported by browsers. React code is transformed into a normal HTML, CSS and JS code that is supported by the browser behind the scene with the help of the build process.

Now, if you look at the `App` component, you will realize that it is nothing but an JS function.

Though as a component, it must follow 2 rules -

1. `Names start with uppercase character`: It must start with an uppercase character. Multi-word should be written in pascal case (e.g. "MyHeader"). It is recommended to use names that best describe UI.
2. `Returns a "Renderable" value`: Components must return a value that can be rendered on the screen. Usually it is JSX code.

### First Custom Component

As components are just JS functions that return JSX code, you can define a function that follows component naming rule and return JSX code from it.

```js
function Header() {
  return (
    <header>
      <img src="src/assets/react-core-concepts.png" alt="Stylized atom" />
      <h1>React Essentials</h1>
      <p>
        Fundamental React concepts you will need for almost any app you are
        going to build!
      </p>
    </header>
  );
}
```

In this `Header` component, you can see I am returning JSX code that looks exactly like a HTML code.

But! unlike JS functions you don't call the component inside react code.

Instead you write this component as a HTML tag along with other HTML tags and react will call this function automatically to build the UI returned from this component.

```js
function App() {
  return (
    <div>
      <Header></Header>
      <main>
        <h2>Time to get started!</h2>
      </main>
    </div>
  );
}
```

Here, you can see that `Header` is used as a tag.

2 ways of including the component code inside HTML-

```js
<Header></Header>
```

OR

```js
<Header />
```

When using the 2nd option, don't forget to use `/` before ending the tag.

**EXTRA NOTE ON JSX**

> `.jsx` is a file extension that's not supported by the browser! It's working because you're working in a React project that supports this special extension. Because this extension "tells" the underlying build process (which is running behind the scenes when the development server is running) that a file contains JSX code (which is also not supported by browsers).
> It's important to understand that it's really just that build process that cares about this extension.
> And therefore, you'll also find React projects that don't use .jsx but instead just .js as a file extension. And in those .js files, you'll also find JSX code. Because it simply depends on the underlying build process which extension is expected when using this JSX syntax in a file.
> Since it doesn't work in the browser either way, there is no hard rule regarding this. Instead, you'll find projects that require .jsx (like the project setup we use in this course) and you'll find projects that also support .js (with JSX code inside).

### How components are handled by React?

As you already know components are written in JSX and they are unsupported in browser. Then how react handles them?

If you look at the source code of the page, then you will notice that no component is present there. Only the script import `index.jsx` is present.

Now, if you look at the code written in `index.jsx` file, you will see that I am importing the `App` from `App.jsx` file and then passing it to `render` method.

`ReactDOM` is the library that is responsible for converting this component code to browser compatible HTML, CSS and JS code. To do that I am rendering this component on root that is created by passing the `root` element created in the `index.html` file to the `create_root` method and rendering the components.

If `App` component has sub components inside it then React will go through each component in the `Component Tree` untill it finds built-in HTML DOM elements.

> That is also the reason why the component names should start with upper-case letters as they will collide with the built-in HTML DOM elements that start with lower-case letters.

### Using and outputing random dynamic values

To output contents dynamically, you can use `{value}` inside of the JSX code.

You can't write for-loop, if-else statments inside the curly braces, you can only use expressions that yield some final value inside it.

Changing the heading paragraph randomly. You can do something like this -

```js
const reactDescriptions = ["Fundamental", "Crucial", "Core"];

function getRandomInt(max) {
  return Math.floor(Math.random() * (max + 1));
}

function Header() {
  const description = reactDescriptions[getRandomInt(2)];

  return (
    <header>
      <img src="src/assets/react-core-concepts.png" alt="Stylized atom" />
      <h1>React Essentials</h1>
      <p>
        {description} React concepts you will need for almost any app you are
        going to build!
      </p>
    </header>
  );
}
```

You should also change any hardcoded paths inside your JSX code to imported values. Otherwise, when react project is deployed the project structure will change and the file paths may not be the correct paths anymore.

This will also make sure that react can load the image more optimally which is not possible right now.

```js
import reactImg from "./assets/react-core-concepts.png";
// Code...

function Header() {
  return (
    <header>
      <img src={reactImg} alt="Stylized atom" />
      Code...
  );
}

```

Here, you can name the image variable any name you want. With this the image will be loaded dynamically with a path that is automatically created by react and points to the correct file even after deployment.

### Component "props" concept

`props` are used to send data to components and reuse the same component to output different values.

To show components with different values but with the same configuration (so to say...), you need to set attributes of this component and pass values for this attributes. The attributes are actually called `prop`. In the component function you take only one argument `props` (you can name it anything you want) and use `props` as a dictionary that gives you key-value pairs. Each prop will be the key for `props` and it's value will be the value for that `props` key.

```js
function CoreConcepts(props) {
  return (
    <li>
      <img src={props.image} alt={props.description} />
      <h3>{props.title}</h3>
      <p>{props.description}</p>
    </li>
  );
}

function App() {
  return (
    <div>
      <Header></Header>
      <main>
        <section id="core-concepts">
          <h2>Core Concepts</h2>
          <ul>
            <CoreConcepts
              title="Component"
              description="The core UI building block."
              image={componentImg}
            />
            <CoreConcepts />
            <CoreConcepts />
            <CoreConcepts />
          </ul>
        </section>
      </main>
    </div>
  );
}
```

**Another ways of handling props**

```js
import reactImg from "./assets/react-core-concepts.png";
import { CORE_CONCEPTS } from "./data";

// Code...

function CoreConcepts({ image, title, description }) {
  return (
    <li>
      <img src={image} alt={description} />
      <h3>{title}</h3>
      <p>{description}</p>
    </li>
  );
}

// Code ...

function App() {
  return (
    <div>
      <Header></Header>
      <main>
        <section id="core-concepts">
          <h2>Core Concepts</h2>
          <ul>
            <CoreConcepts {...CORE_CONCEPTS[0]} />
            <CoreConcepts {...CORE_CONCEPTS[1]} />
            <CoreConcepts {...CORE_CONCEPTS[2]} />
            <CoreConcepts {...CORE_CONCEPTS[3]} />
          </ul>
        </section>
      </main>
    </div>
  );
}

// Code...
```

**Best Practices for Components**

1. Create a `components` folder inside the `src` folder.
2. Seperate the `Header` and `CoreConcept` component and move them inside `Header.jsx` and `CoreConcept.jsx` file respectively.
3. Don't forget to change the imports path. Now, the paths will be `../` instead of `./`, if you are using any images, styles.
4. You can also split the style for each component and save the jsx and css together inside a folder that has the same name an the component. Though it comes with preferences and you might not choose to save them inside a folder.

### Composition props

Sometimes it makes more sense to use react components as HTML tags and pass information between the opening and closing tags like -

```html
<button>BUTTON</button>
```

Fortunately, react has a concept of composition props where you can access the contents written between the opening and closing tags. The content can be accessed using `children` property of the props which is always passed to components.

To add a button component and show texts written in between the components, you need to do something like this -

```js
export default function TabButton({ children }) {
  return (
    <li>
      <button>{children}</button>
    </li>
  );
}
```

In the `App.jsx` you can use the `TabButton` component like this -

```js
function App() {
  return (
    <div>
      Code...
      <main>
        Code...
        <section id="examples">
          <h2>Examples</h2>
          <menu>
            <TabButton>Components</TabButton>
            <TabButton>JSX</TabButton>
            <TabButton>Props</TabButton>
            <TabButton>State</TabButton>
          </menu>
        </section>
      </main>
    </div>
  );
}
```

### Handling events in React

In vanilla JS, you have to write imperative code where you use `addEventListener` to listen to events and call a function whenever that function is triggered.

```js
buttonElm.addEventListener("click", () => {
  // Do something
});
```

But, in react you don't have to write such code, instead here you have to write declarative code which means you want to run a function when a click occurs but that will handle by react only.

To achieve this, you have access to some attributes that are available in any built-in HTML DOM element, which is also a react component that browser understands.

For handling click event on a button, you can use `onClick` attribute and pass a function that handles the event.

```js
export default function TabButton({ children }) {
  function handleClick() {
    console.log("Hello World!");
  }

  return (
    <li>
      <button onClick={handleClick}>{children}</button>
    </li>
  );
}
```

You can write _on_ and then press `CTRL`+`SPACE` to see all available attributes that will be triggered for different events.

### Passing functions as values to props

Sometimes you would want to change content in your App depending on some events. But, if you have used components that how would you handle that event and show dynamic content based on the event?

It would be better if we have the same `onClick` or similar attribute available for components as well. But we don't have those.

Instead what react developers usually follow is, they pass the function that handles the event and shows dynamic content in the App as a prop to the component. The component then uses this function as a value to `onClick` or similar attribute which is available for HTML built-in elements.

By doing so, you are essentially calling the handler defined in your app component which is responsible for showing dynamic contents based on the component triggered.

```js
function App() {
  function handleSelect() {
    console.log("Hello World! -- Selected...");
  }

  return (
    <div>
      Code ...
      <main>
        Code ...
        <section id="examples">
          <h2>Examples</h2>
          <menu>
            <TabButton onSelect={handleSelect}>Components</TabButton>
            <TabButton onSelect={handleSelect}>JSX</TabButton>
            <TabButton onSelect={handleSelect}>Props</TabButton>
            <TabButton onSelect={handleSelect}>State</TabButton>
          </menu>
        </section>
      </main>
    </div>
  );
}
```

```js
export default function TabButton({ children, onSelect }) {
  return (
    <li>
      <button onClick={onSelect}>{children}</button>
    </li>
  );
}
```

### Passing custom arguments to event functions

Sometimes you would want to pass values to the event functions so that they can dynamically show contents based on which component was selected. To do that instead of passing the handler directly to `onClick`, you have to create an arrow function and call the handler when that arrow function is called.

As the arrow function is a JS value, it will be passed as a value to `onClick` and when click event occurs, it will execute the code inside this function which means it will call the handler function.

```js
<TabButton onSelect={() => handleSelect("components")}>
              Components
</TabButton>
<TabButton onSelect={() => handleSelect("jsx")}>JSX</TabButton>
<TabButton onSelect={() => handleSelect("props")}>Props</TabButton>
<TabButton onSelect={() => handleSelect("state")}>State</TabButton>
```

```js
function handleSelect(selectedButton) {
  console.log(selectedButton);
}
```

### State (**useState**)

When you want to dyanmically change the contents of the react components you need to "tell" react app to reload so that it will execute the components and render them on UI again.

It is not possible by just changing a variable when an event happens and making it dynamically loading it in react component code.

Instead, you have to use `State` of a variable and "tell" react to render UI again because the state has changed.

To do that you have to use `useState` function from react. It is called a react hook.

React hooks start with `use` keyword. Here, the react hook that you will be using is the `useState` hook that

2 rules of hooks -

1. Only call hooks inside of component functions. It must not be called outside react components.
2. Only call hooks on the top level. They must not be called inside nested code statements.

### Showing conditional components

```js
let tabContent = <p>Please click any topic.</p>;

if (selectedTopic) {
  tabContent = (
    <div id="tab-content">
      <h3>{EXAMPLES[selectedTopic].title}</h3>
      <p>{EXAMPLES[selectedTopic].description}</p>
      <pre>
        <code>{EXAMPLES[selectedTopic].code}</code>
      </pre>
    </div>
  );
}

return (
  <div>
    <Header></Header>
    <main>
      Code...
      <section id="examples">
        <h2>Examples</h2>
        <menu>Code...</menu>
        {tabContent}
      </section>
    </main>
  </div>
);
```

OR

```js
{
  !selectedTopic && <p>Please click any topic.</p>;
}
{
  selectedTopic && (
    <div id="tab-content">
      <h3>{EXAMPLES[selectedTopic].title}</h3>
      <p>{EXAMPLES[selectedTopic].description}</p>
      <pre>
        <code>{EXAMPLES[selectedTopic].code}</code>
      </pre>
    </div>
  );
}
```

### CSS dynamic styling

_TabButton.jsx_

```js
export default function TabButton({ children, onSelect, isSelected }) {
  return (
    <li>
      <button className={isSelected ? "active" : undefined} onClick={onSelect}>
        {children}
      </button>
    </li>
  );
}
```

_App.jsx_

```js
<menu>
  <TabButton
    isSelected={selectedTopic === "components"}
    onSelect={() => handleSelect("components")}
  >
    Components
  </TabButton>
  <TabButton
    isSelected={selectedTopic === "jsx"}
    onSelect={() => handleSelect("jsx")}
  >
    JSX
  </TabButton>
  <TabButton
    isSelected={selectedTopic === "props"}
    onSelect={() => handleSelect("props")}
  >
    Props
  </TabButton>
  <TabButton
    isSelected={selectedTopic === "state"}
    onSelect={() => handleSelect("state")}
  >
    State
  </TabButton>
</menu>
```

### Output lists dynamically

JSX code also outputs list with JSX code, so we can do something like this to dynamically load lists -

```js
{
  CORE_CONCEPTS.map((coreConcept) => (
    <CoreConcepts key={coreConcept.title} {...coreConcept} />
  ));
}
```

This is much more efficient, scalable.

## React Essentials - Deep Dive

### You don't have to use JSX code!

Instead of using JSX code, you can also create components by `React.createElement` method.

Instead of creating components like this,

```js
<div id="content">
  <p>Hello World!</p>
</div>
```

You can use `createElement` method,

```js
React.createElement(
  "div",
  { id: "content" },
  React.createElement("p", null, "Hello World!")
);
```

But, as you can see the code will become more tedious! Instead we can just rely on react build process to convert our JSX code to browser compatible code.

```js
React.createRoot(document.getElementById("root")).render(
  React.createElement(App)
);
```

### Fragments

You might have noticed that in the react code you wrap the components inside a div and then return it, if it containes more than one component.

It is normally how javascript works, in JS you can't return 2 values (not talking about any array type structure).

So when you write the code like this-

```js
return (
  <Header />
  <CoreConcepts />
);
```

It will give you an error.

But, you also don't want to bother with extra elements in your dom which are unnecessary.

To achieve the thing you want, you can user `Fragment`.

```js
import { Fragment } from "react";

return (
  <Fragment>
    <Header />
    <CoreConcepts />
  </Fragment>
);
```

Ans modern react also supports `<> </>` as a wrapper which works same as `Fragment`.

```js
return (
  <>
    <Header />
    <CoreConcepts />
  </>
);
```

In both methods, you will notice that no extra element is created in the DOM.

### Spliting components

In large code bases you can't make one component responsible for taking care of multiple things.

It will lead to problems. If a component handles state as well as other dynamic components, due to the state change of one component, your entire component will run again along with all other sub-components inside it - Which might result in different UI for other components if they handle dynamic things.

### Forwarding props / Proxy props

If you try to set props (like `id`, `className`) on components, they are not automatically attached to the components built-in element `id` or `className`.

```js
export default function Section({ title, children }) {
  return (
    <section>
      <h2>{title}</h2>
      {children}
    </section>
  );
}
```

```js
return (
  <Section id="examples" title="Examples">
    <menu>
      <TabButton
        isSelected={selectedTopic === "components"}
        onSelect={() => handleSelect("components")}
      >
        Components
      </TabButton>
      <TabButton
        isSelected={selectedTopic === "jsx"}
        onSelect={() => handleSelect("jsx")}
      >
        JSX
      </TabButton>
      <TabButton
        isSelected={selectedTopic === "props"}
        onSelect={() => handleSelect("props")}
      >
        Props
      </TabButton>
      <TabButton
        isSelected={selectedTopic === "state"}
        onSelect={() => handleSelect("state")}
      >
        State
      </TabButton>
    </menu>
    {tabContent}
  </Section>
);
```

![Broken due to props not forwared](images/1.png)

To forward the props to sub-components, you can use `...props` and just put it inside the component where you want to use those `props` passed from parent component.

```js
export default function Section({ title, children, ...props }) {
  return (
    <section {...props}>
      <h2>{title}</h2>
      {children}
    </section>
  );
}
```

### Using multiple JSX slots

Sometimes you wanna build components that are reusable in every scenarios but unlike normal components where you can send `children` as a value to the component to use any other JSX code inside the component declaration, you might need to send another JSX code that will be used somewhere else in the component and can't be used along with the passed `children`.

You can do something like the following-

```js
export default function Tabs({ children, buttons }) {
  return (
    <>
      <menu>{buttons}</menu>
      {children}
    </>
  );
}
```

You can pass the extra JSX and slot for that-

```js
return (
  <Section id="examples" title="Examples">
    <Tab
      buttons={
        <>
          <TabButton
            isSelected={selectedTopic === "components"}
            onClick={() => handleSelect("components")}
          >
            Components
          </TabButton>
          <TabButton
            isSelected={selectedTopic === "jsx"}
            onClick={() => handleSelect("jsx")}
          >
            JSX
          </TabButton>
          <TabButton
            isSelected={selectedTopic === "props"}
            onClick={() => handleSelect("props")}
          >
            Props
          </TabButton>
          <TabButton
            isSelected={selectedTopic === "state"}
            onClick={() => handleSelect("state")}
          >
            State
          </TabButton>
        </>
      }
    >
      {tabContent}
    </Tab>
  </Section>
);
```

### Using components dynamically inside another component

Sometimes you might want to wrap some element inside a `div`, `ul`, `menu`, or any custom component. But how do you do that?

To achieve that, you use the pattern where you pass the component as a `prop` and use this `prop` as a component inside the component.

```js
 <Tabs
        ButtonsContainer="menu"
        // ...
>
ABCD
</Tab>
```

```js
export default function Tabs({ children, buttons, ButtonsContainer }) {
  return (
    <>
      <ButtonsContainer>{buttons}</ButtonsContainer>
      {children}
    </>
  );
}
```

OR

```js
 <Tabs
        buttonsContainer="menu"
        // ...
>
ABCD
</Tab>
```

```js
export default function Tabs({ children, buttons, buttonsContainer }) {
  const ButtonsContainer = buttonsContainer;
  return (
    <>
      <ButtonsContainer>{buttons}</ButtonsContainer>
      {children}
    </>
  );
}
```

Make sure that you pass built-in elements as string and custom components as dynamic value `ButtonsContainer={Section}`.

### Folders: src, public

**The `public/` Folder**

You can store images in the `public/` folder and then directly reference them from inside your `index.html` or `index.css` files.

The reason for that is that images (or, in general: files) stored in `public/` are made publicly available by the underlying project development server & build process. Just like `index.html`, those files can directly be visited from inside the browser and can therefore also be requested by other files.

If you try loading `localhost:5173/some-image.jpg`, you'll be able to see that image (if it exists in the `public/` folder, of course).

**The `src/assets/` Folder**

You can also store images in the `src/assets/` folder (or, actually, anywhere in the src folder).

So what's the difference compared to `public/`?

Any files (of any format) stored in `src` (or subfolders like `src/assets/`) are not made available to the public. They can't be accessed by website visitors. If you try loading `localhost:5173/src/assets/some-image.jpg`, you'll get an error.

Instead, files stored in `src/` (and subfolders) can be used in your code files. Images imported into code files are then picked up by the underlying build process, potentially optimized, and kind of "injected" into the `public/` folder right before serving the website. Links to those images are automatically generated and used in the places where you referenced the imported images.

**Which Folder Should You Use?**
You should use the `public/` folder for any images that should not be handled by the build process and that should be generally available. Good candidates are images used directly in the `index.html` file or favicons.

On the other hand, images that are used inside of components should typically be stored in the `src/` folder (e.g., in `src/assets/`).

### React components work in isolation

Look at the code below -

```js
import { useState } from "react";

export default function Player({ name, symbol }) {
  const [editing, setEditing] = useState(false);
  const [newName, setNewName] = useState(name);

  function handleClick() {
    if (!editing) {
      // Start editing
      setEditing(true);
    } else {
      // Save the changes
      setEditing(false);
    }
  }

  return (
    <li>
      <span className="player">
        {!editing && <span className="player-name">{newName}</span>}
        {editing && (
          <input
            htmlFor="playername"
            type="text"
            required
            onChange={(e) => setNewName(e.target.value)}
          ></input>
        )}
        <span className="player-symbol">{symbol}</span>
      </span>
      <button onClick={handleClick}>{editing ? "Save" : "Edit"}</button>
    </li>
  );
}
```

I am using `Player` component 2 times in the `App.jsx` file -

```js
import Player from "./components/Player";

function App() {
  return (
    <main>
      <div id="game-container">
        <ol id="players">
          <Player name="Player 1" symbol="X" />
          <Player name="Player 2" symbol="O" />
        </ol>
        GAME BOARD
      </div>
      LOG
    </main>
  );
}

export default App;
```

In the UI, even though I click on one `Player` component and edit the name of the player, it won't interfere with the other one.

Because react makes sure that the elements in the DOM, even though are same component, are instances of the same component but work in isolation.
Component instances in the DOM won't interfere with other instance in the DOM that are created from the same component.

### Best practice: Update the state based on it's old state

It is important to remember that react always schedules the state updates in near future and not immediately.

```js
const [isEditing, setIsEditing] = useState(false);

function handleClick() {
  setIsEditing(!isEditing);
}
```

When `handleClick` function is triggered, react schedules a state update in future where the `isEditing` has a `false` value and it sets the new value as `true` (negation operator).

If I do something like this -

```js
const [isEditing, setIsEditing] = useState(false);

function handleClick() {
  setIsEditing(!isEditing); // 1
  setIsEditing(!isEditing); // 2
}
```

In the `1`, react schedules a state update where `isEditing` is `false`, `2` also does the same thing!
Unlike how you might expect that `2` will have the `isEditing` as `true` because of `1`.

That is why the best practice is to pass a function to `setIsEditing` function instead of passing a value.

Because the function gets access to `isEditing` value in the current time, it will use the new `isEditing` value after `1`.

```js
const [isEditing, setIsEditing] = useState(false);

function handleClick() {
  setIsEditing((editing) => !editing);
}
```

### Two-way handling

Sometimes we would want to feed the value of an input field and use that value to change the value of the input field.

This is called two-way handling.

To do that-

```js
export default function Player({ initialName, symbol }) {
  const [playerName, setPlayerName] = useState(initialName);
  const [isEditing, setIsEditing] = useState(false);

  function handleClick() {
    setIsEditing((editing) => !editing);
  }

  let editablePlayerName = <span className="player-name">{playerName}</span>;

  function handleEdit(event) {
    setPlayerName(event.target.value);
  }

  if (isEditing) {
    editablePlayerName = (
      <input
        htmlFor="playername"
        type="text"
        required
        value={playerName}
        onChange={handleEdit}
      ></input>
    );
  }

  return (
    <li>
      <span className="player">
        {editablePlayerName}
        <span className="player-symbol">{symbol}</span>
      </span>
      <button onClick={handleClick}>{isEditing ? "Save" : "Edit"}</button>
    </li>
  );
}
```

I have to use `onChange` prop of the `input` element. Keep track of the state of the input value, when the change happens, I set the new value of the state and that value will be the new value of the input field instead of the `initialName`.

### Update object state immutabely

Take an example here-

```js
import { useState } from "react";
const initialGameBoard = [
  [null, null, null],
  [null, null, null],
  [null, null, null],
];

export default function GameBoard() {
  const [gameBoard, setGameBoard] = useState(initialGameBoard);

  function handleSquareSelection(rowIndex, colIndex) {
    setGameBoard((prevGameBoard) => {
      // Change the prevGameBoard by replacing null with X/O
    });
  }

  return (
    <ol id="game-board">
      {gameBoard.map((row, rowIndex) => (
        <li key={rowIndex}>
          <ol>
            {row.map((playerSymbol, colIndex) => (
              <li key={colIndex}>
                <button
                  onClick={() => handleSquareSelection(rowIndex, colIndex)}
                >
                  {playerSymbol}
                </button>
              </li>
            ))}
          </ol>
        </li>
      ))}
    </ol>
  );
}
```

If I change the `prevGameBoard`, it will directly change the object placed at memory. If this same object state is used in different places it could lead to complex bugs, so it is recommended to always use a copy of the passed reference values by destructuring.

```js
setGameBoard((prevGameBoard) => {
  const newGameBoard = [...prevGameBoard.map((rows) => [...rows])];
  newGameBoard[rowIndex][colIndex] = "X";
  return newGameBoard;
});
```

[Reference vs Primitives](https://academind.com/tutorials/reference-vs-primitive-values)

### Lift states up

Sometimes you might want to use a state and depending on the state you want to change the UI. But what if 2 components need the same state at the same time to change it. How do you do that?

When you face such situations, you have to look for the closest common ancestor of both components and use the state there. Then, you can pass this state as `prop` to the child components.

### Prefer computed values than unnessecary state management

Some of the points need to be remembered-

1. Try to avoid states that more or less store the same data with the only difference the other one stores more or less. In this case, try to optimize your code such that you can use just 1 state throughout different child components.
2. Do not try to use 2 states at the same time when setting one state only. Try to compute a similar value that is equal to that state value.

```js
import { useState } from "react";
import Player from "./components/Player";
import GameBoard from "./components/GameBoard";

function App() {
  const [gameTurns, setGameTurns] = useState([]);
  const [activePlayer, setActivePlayer] = useState("X");

  function handleSelectSquare() {
    setActivePlayer((prevActivePlayer) =>
      prevActivePlayer == "X" ? "O" : "X"
    );
  }

  // Code...
}

export default App;
```

```js
import { useState } from "react";
const initialGameBoard = [
  [null, null, null],
  [null, null, null],
  [null, null, null],
];

export default function GameBoard({ playerSymbol, onSelectSquare }) {
  // WE DO NOT NEED GAMEBOARD AS GAMETURNS ALREADY STORES SIMILAR DATA
  //   const [gameBoard, setGameBoard] = useState(initialGameBoard);

  //   function handleSquareSelection(rowIndex, colIndex) {
  //     setGameBoard((prevGameBoard) => {
  //       const newGameBoard = [...prevGameBoard.map((rows) => [...rows])];
  //       newGameBoard[rowIndex][colIndex] = playerSymbol;
  //       return newGameBoard;
  //     });
  //     onSelectSquare();
  //   }

  return (
    <ol id="game-board">
      {gameBoard.map((row, rowIndex) => (
        <li key={rowIndex}>
          <ol>
            {row.map((playerSymbol, colIndex) => (
              <li key={colIndex}>
                <button
                  onClick={() => handleSquareSelection(rowIndex, colIndex)}
                >
                  {playerSymbol}
                </button>
              </li>
            ))}
          </ol>
        </li>
      ))}
    </ol>
  );
}
```

Instead of doing this -

```js
import { useState } from "react";
import Player from "./components/Player";
import GameBoard from "./components/GameBoard";

function App() {
  const [gameTurns, setGameTurns] = useState([]);
  const [activePlayer, setActivePlayer] = useState("X");

  function handleSelectSquare(rowIndex, colIndex) {
    setActivePlayer((prevActivePlayer) =>
      prevActivePlayer == "X" ? "O" : "X"
    );

    setGameTurns((prevTurns) => {
      let currentPlayer = "X";

      if (prevTurns.length > 0 && prevTurns[0].player == "X") {
        currentPlayer = "O";
      }

      const updatedTurns = [
        { square: { row: rowIndex, col: colIndex }, player: currentPlayer },
        ...prevTurns,
      ];

      return updatedTurns;
    });
  }

  return (
    <main>
      <div id="game-container">
        <ol id="players" className="highlight-player">
          <Player
            initialName="Player 1"
            symbol="X"
            isActive={activePlayer == "X"}
          />
          <Player
            initialName="Player 2"
            symbol="O"
            isActive={activePlayer == "O"}
          />
        </ol>

        <GameBoard
          onSelectSquare={handleSelectSquare}
          playerSymbol={gameTurns[0].player}
        />
      </div>
      LOG
    </main>
  );
}

export default App;
```

## Styling React Components

### Disadvantages of CSS

1. You need to know CSS.
2. CSS rules are not scoped to components. Different components may use same CSS class name which had different purpose in both components and they clash with each other.

When you make changes to the styles part of one component, if that element is used in other components then they will be styled in the same way, if both are used together. Because when the page loads after building process, the css files needed for components are also loaded, so all styles become common to all elements in the page irrespective of which component it was isolated with.

### Inline style

One solution to the problem could be inline styling of the components.

To use inline styles, you can use the `style` property of the components and add the styles as an object.

```js
<p
  style={{
    color: "red",
    textAlign: "left",
  }}
>
  A community of artists and art-lovers.
</p>
```

If you have a style property like `text-align`, make sure to use string or camel case to write the property name.
Values should be in the form of a string.

Advantages of using inline style-

1. Fast and simple styling.
2. Dynamic(conditionally) styling is simple and easy.
3. Styles only effect the elements on which you apply them.

Disadvantages -

1. You need to style every individual element.
2. No seperation b/w JSX and CSS code.

### Dynamic Styling with CSS Files and Classes

When adding dynamic class using `className` property, make sure to add a ternary exp that sets the `className` to `undefined` is the condition does not met, otherwise browser will throw a warning saying that "false was not a className".

Do this

```js
<input
  type="email"
  className={emailNotValid ? "invalid" : undefined}
  onChange={(event) => handleInputChange("email", event.target.value)}
/>
```

Instead of doing this

```js
<input
  type="email"
  className={emailNotValid && "invalid"}
  onChange={(event) => handleInputChange("email", event.target.value)}
/>
```

If you want to add a `className` to a component along with a mandetory `className` conditionally, then you can use JS special string literals that use `${}` sign.

```js
<label className={`label ${emailNotValid ? "invalid" : ""}`}>Email</label>
```

## Refs and Portals

### Why ref?

There are some situations where you may want to access the DOM element that are not inside the component. Using that DOM element's data you wanna change the UI when the component is triggered.

Like the following case where you are trying to set name of the player when he/she clicks on the button and the name would be the value in the input element.

```js
import { useState } from "react";

export default function Player() {
  const [playerName, setPlayerName] = useState();
  const [submitted, setSubmitted] = useState(false);

  function handleChangeName(e) {
    setSubmitted(false);
    setPlayerName(e.target.value);
  }

  function handleSetName() {
    setSubmitted(true);
  }

  return (
    <section id="player">
      <h2>Welcome {submitted ? playerName : "unknown entity"}</h2>
      <p>
        <input type="text" onChange={handleChangeName} value={playerName} />
        <button onClick={handleSetName}>Set Name</button>
      </p>
    </section>
  );
}
```

But the problem here is, whenever you edit the input, the heading name will change back to "unknown entity". If you want to keep it as it is and then change the name when again `Set Name` is clicked then you have to use some more states. This is the issue here. Although you can introduce more states to manage this issue, but still makes the code too long.

This is where `ref` comes in play.

### What is ref?

`ref` simply is a prop that is available in every react component. To use `ref`, you need to first create a `ref` instance from react using `useRef()` function. After you assign the `ref` for the component, the `ref` object will get access to all HTML properties available to that element.

```js
import { useState, useRef } from "react";

export default function Player() {
  const playerNameRef = useRef();

  const [playerName, setPlayerName] = useState();

  function handleSetName() {
    setPlayerName(playerNameRef.current.value);
  }

  return (
    <section id="player">
      <h2>Welcome {playerName || "unknown entity"}</h2>
      <p>
        <input ref={playerNameRef} type="text" />
        <button onClick={handleSetName}>Set Name</button>
      </p>
    </section>
  );
}
```

To access any property from the DOM use `ref.current` which holds all properties. `current` is the only property available to all all `ref` objects.

Now, if you want to clear the input field whenever the user clicks on the Set Name button you can use `ref` to change the input value directly.

> Although here it may seem like we are violating the rule of React for imperative code and not declarative code, because we are directly changing the input field value from react. But if you see that this saves a lot of code then you can go with it. Keeping in mind to not manipulate DOM using refs to avoid writing states because that is not the way React should work.

### Use cases of ref

Another use case of `ref`: When you have a timer that stores the id of a `setTimeout` function. You want to pass this timer id to `cleanTimerout` so that the time out function stops without expiring.

If you use states you will re-evaluate the whole component when the state value changes, which you don't want because it does not effect the UI much.

If you use a variable to take care of it, then the variable will be reset when any state changes and component re-evaluates.

If you use global variable then it is shared among all other components and if some other instance of the component run after the current instance then the global variable will point to another timer id and the current timer id will be lost.

`ref` is handy in this case because it does not re-evaluate the component when value is changed, it maintains it's value even after component re-evaluates and it is different for all instances of the component.

```js
export default function TimerChallenge({ title, targetTime }) {
  const timer = useRef();

  const [timerStarted, setTimerStarted] = useState(false);
  const [timerExpired, setTimerExpired] = useState(false);

  function handleStart() {
    timer.current = setTimeout(() => {
      setTimerExpired(true);
    }, targetTime * 1000);
    setTimerStarted(true);
  }

  function handleStop() {
    clearTimeout(timer.current);
  }

  return (
    <>
      {timerExpired && <ResultsModal result={"lost"} targetTime={targetTime} />}
      <section className="challenge">
        <h1>{title}</h1>
        {timerExpired ? <p>You lost.</p> : ""}
        <p className="challenge-time">
          {targetTime} second{targetTime > 1 ? "s" : ""}
        </p>
        <p>
          <button onClick={timerStarted ? handleStop : handleStart}>
            {timerStarted ? "Stop" : "Start"} Challenge
          </button>
        </p>
        <p className="">
          {timerStarted ? "Time is running..." : "Timer Inactive"}
        </p>
      </section>
    </>
  );
}
```

### Forward ref

Let's take a situation where you want to access a components DOM element and decide to manipulate it based on the current components calculation.

You want to show a modal when the user timer expires and let the user know whether he won or lost. There is a component that shows the modal `ResultsModal` and another component `TimerChallenge` that starts and stops the timer.

In HTML, there is `dialog` element that helps to open modal but the problem is if not opened programmatically, it will not overlay the background properly.

```js
export function ResultsModal({ result, targetTime }) {
  return (
    <dialog className="result-modal" open>
      <h2>You {result}</h2>
      <p>
        Your target time was <strong>{targetTime} seconds.</strong>
      </p>
      <p>
        You stopped the timer with <strong>X seconds left.</strong>
      </p>
      <form method="dialog">
        <button>Close</button>
      </form>
    </dialog>
  );
}
```

The `open` attribute will focefully open the modal and not overlay the background.

Instead to do it properly, we need to show it conditionally and with programming logic.

For that we need to use `ref` so that we can get the access of the `dialog` element and use the `showModal` function to show the modal and also show the overlay.

To use `ref` and pass them to custom component, we can't just pass it as a `prop`, it will give error.

To do the above, we need to use `forwardRef` method to wrap the `ResultsModal` component and get access to the `ref`.

After doing that, we can pass the `ref` value as prop to the custom component.

```js
import { forwardRef } from "react";

const ResultsModal = forwardRef(({ result, targetTime }, ref) => {
  return (
    <dialog ref={ref} className="result-modal" open>
      <h2>You {result}</h2>
      <p>
        Your target time was <strong>{targetTime} seconds.</strong>
      </p>
      <p>
        You stopped the timer with <strong>X seconds left.</strong>
      </p>
      <form method="dialog">
        <button>Close</button>
      </form>
    </dialog>
  );
});

export default ResultsModal;
```

`forwardRef` returns a value so instead of a function, we need to catch it inside a variable and export it later.

`forwardRef` also gives access to `ref` as a paramter to the component defined inside.

### useImperativeHandle

Sometimes when developing a large project, you might wanna make the usage of the forwardRef component simpler for users who are using it by just introducing a simple `open` function that opens the modal and does not let the user think too much about the way this modal is called.

Because right now, we are using `dialog` element which comes with `showModal` method but later if the owner changes the `dialog` to `div` then it will break the working of users who were doing it using `showModal`.

To achieve the above, we can use `useImperativeHandle` that takes the `ref` from outside and links this `ref` to the object returned by it. The object will contain `open` that is a general function used for opening the modal no matter of the implementation specification.

```js
import { forwardRef, useImperativeHandle, useRef } from "react";

const ResultsModal = forwardRef(
  ({ targetTime, remainingTime, onReset }, ref) => {
    const dialog = useRef();

    const userLost = remainingTime <= 0;
    const formattedRemainingTime = (remainingTime / 1000).toFixed(2);

    useImperativeHandle(ref, () => ({
      open() {
        dialog.current.showModal();
      },
    }));

    return (
      <dialog ref={dialog} className="result-modal">
        {userLost && <h2>You lost</h2>}
        <p>
          Your target time was <strong>{targetTime} seconds.</strong>
        </p>
        <p>
          You stopped the timer with{" "}
          <strong>{formattedRemainingTime} seconds left.</strong>
        </p>
        <form method="dialog" onSubmit={onReset}>
          <button>Close</button>
        </form>
      </dialog>
    );
  }
);

export default ResultsModal;
```

### Portal

Like the example we have been talking about so far, there could be react elements that you might want render somewhere else than where it is placed by default by the DOM.

The dialog component in the example is rendered near the `challenges` section because that is where it is called.

But, technically it should not be there. It is not a functional detail but a technical detail that might help you avoid unnecessary styling conflicts as it becomes part of some other DOM element which could be nested.

That is where portals come into picture. Just the name says, it teleports the component somewhere else in the DOM with where the developer wants it to be.

Unlike other functions so far, to create portal you need something called `createPortal` imported from `react-dom` and not `react`!

It takes 2 arguments, first is the component JSX code and second is the DOM element where you want to put the JSX code into.

Here, there is a `div` with the id `modal`, you can target it using JS function `document.getElementById` to get the element using the id.

```js
// ...
import { createPortal } from "react-dom";

const ResultsModal = forwardRef(
  ({ targetTime, remainingTime, onReset }, ref) => {
    // ...

    return createPortal(
      <dialog ref={dialog} className="result-modal" onClose={onReset}>
        {userLost && <h2>You lost</h2>}
        {!userLost && <h2>Your score {score}</h2>}
        <p>
          Your target time was <strong>{targetTime} seconds.</strong>
        </p>
        <p>
          You stopped the timer with{" "}
          <strong>{formattedRemainingTime} seconds left.</strong>
        </p>
        <form method="dialog" onSubmit={onReset}>
          <button>Close</button>
        </form>
      </dialog>,
      document.getElementById("modal")
    );
  }
);

export default ResultsModal;
```

## Context API and useReducer

Domo application: [Advanced state management using context API](https://codesandbox.io/p/sandbox/adv-state-mgmt-context-start-forked-v42lsq)

### Problems with normal state management

Sometimes state management becomes very messy when there lots of components that are passing the same state without doing anything with it inside the component body, just forwarding the state without doing anything to it.

This issue is called prop drilling where you just drill the state till the component that uses it.

To fix this issue, we are going to use context API and useReducer.

### React context API

Context API provides the facility to share the same state accross multiple components without sending the states to next components. The best thing about context API is that it can belink with the state that is needed for components, and possibly all components get the facility to use it whenever they want.

### Creating context API

To create a context, first make a folder `store` (convention but not necessary) that stores the contexts and other data for all the components.

_shopping-cart-context.jsx_

```js
import { createContext } from "react";

export const CartContext = createContext({
  items: [],
});
```

In React context API, there is a provider who provides the context and consumer who consumes the context.

For creating the provider, you have to wrap the component within `ContextComp.Provider` component.

_App.jsx_

```js
import { CartContext } from "./store/shopping-cart-context.jsx";

function App() {
  // ... Code
  return (
    <CartContext.Provider>
      <Header
        cart={shoppingCart}
        onUpdateCartItemQuantity={handleUpdateCartItemQuantity}
      />
      <Shop onAddItemToCart={handleAddItemToCart} />
    </CartContext.Provider>
  );
}
```

Now, to use this context in other components we need 2 things - `useContext` and `CartContext`.

_Cart.jsx_

```js
import { useContext } from "react";
import { CartContext } from "../store/shopping-cart-context.jsx";

export default function Cart({ onUpdateItemQuantity }) {
  const { items } = useContext(CartContext);

  const totalPrice = items.reduce(
    (acc, item) => acc + item.price * item.quantity,
    0
  );
  const formattedTotalPrice = `$${totalPrice.toFixed(2)}`;

  return (
    <div id="cart">
      {items.length === 0 && <p>No items in cart!</p>}
      {items.length > 0 && (
        <ul id="cart-items">
          {items.map((item) => {
            const formattedPrice = `$${item.price.toFixed(2)}`;

            return (
              <li key={item.id}>
                <div>
                  <span>{item.name}</span>
                  <span> ({formattedPrice})</span>
                </div>
                <div className="cart-item-actions">
                  <button onClick={() => onUpdateItemQuantity(item.id, -1)}>
                    -
                  </button>
                  <span>{item.quantity}</span>
                  <button onClick={() => onUpdateItemQuantity(item.id, 1)}>
                    +
                  </button>
                </div>
              </li>
            );
          })}
        </ul>
      )}
      <p id="cart-total-price">
        Cart Total: <strong>{formattedTotalPrice}</strong>
      </p>
    </div>
  );
}
```

### Linking state to context

Right now, the value of the context will always be same as we are directly passing a value. To change the values depending on whether an item is clicked and added to the cart, you have to use the state and set the value to the state so that the context linked state modifies along the actual state.

In the value prop you can also pass the functions that you want to use in different components.

_Product.jsx_

```js
import { CartContext } from "../store/shopping-cart-context.jsx";
import { useContext } from "react";

export default function Product({ id, image, title, price, description }) {
  const { onAddToCart } = useContext(CartContext);

  return (
    <article className="product">
      <img src={image} alt={title} />
      <div className="product-content">
        <div>
          <h3>{title}</h3>
          <p className="product-price">${price}</p>
          <p>{description}</p>
        </div>
        <p className="product-actions">
          <button onClick={() => onAddToCart(id)}>Add to Cart</button>
        </p>
      </div>
    </article>
  );
}
```

_App.jsx_

```js
import { CartContext } from "./store/shopping-cart-context.jsx";

function App() {
  // ... Code
  const cartCtx = {
    items: shoppingCart.items,
    onAddToCart: handleAddItemToCart,
  };

  return (
    <CartContext.Provider value={cartCtx}>
      <Header
        cart={shoppingCart}
        onUpdateCartItemQuantity={handleUpdateCartItemQuantity}
      />
      <Shop>
        {DUMMY_PRODUCTS.map((product) => (
          <li key={product.id}>
            <Product {...product} />
          </li>
        ))}
      </Shop>
    </CartContext.Provider>
  );
}
```

## Taking care of side effects and useEffect hook

### What are side effects?

Side effects are those `tasks` that does not directly impact your component render cycle but still needs to be executed.

In the demo app [side effects forked at codesandbox](https://codesandbox.io/p/sandbox/effects-start-forked-c55gmq), we want to sort the selected places by their distance from the user. To do that, I need to know the exact location of the user and then use the `sortSelectedPlaces` method using the places selected and the location of the user.

Every component in React has a goal of returning some JSX code which will be rendered on the user screen. Anything related to that is important but anything other than that is called a side effect.

_App.jsx_

```js
function App() {
  const modal = useRef();
  const selectedPlace = useRef();
  const [pickedPlaces, setPickedPlaces] = useState([]);

  // Side Effect - sorting places based on user current location
  navigator.geolocation.getCurrentPosition((position) => {
    const sortedPlaces = sortPlacesByDistance(
      AVAILABLE_PLACES,
      position.coords.latitude,
      position.coords.longitude
    );
  });

  function handleStartRemovePlace(id) {
    modal.current.open();
    selectedPlace.current = id;
  }

  function handleStopRemovePlace() {
    modal.current.close();
  }

  function handleSelectPlace(id) {
    setPickedPlaces((prevPickedPlaces) => {
      if (prevPickedPlaces.some((place) => place.id === id)) {
        return prevPickedPlaces;
      }
      const place = AVAILABLE_PLACES.find((place) => place.id === id);
      return [place, ...prevPickedPlaces];
    });
  }

  function handleRemovePlace() {
    setPickedPlaces((prevPickedPlaces) =>
      prevPickedPlaces.filter((place) => place.id !== selectedPlace.current)
    );
    modal.current.close();
  }

  return (
    <>
      <Modal ref={modal}>
        <DeleteConfirmation
          onCancel={handleStopRemovePlace}
          onConfirm={handleRemovePlace}
        />
      </Modal>

      <header>
        <img src={logoImg} alt="Stylized globe" />
        <h1>PlacePicker</h1>
        <p>
          Create your personal collection of places you would like to visit or
          you have visited.
        </p>
      </header>
      <main>
        <Places
          title="I'd like to visit ..."
          fallbackText={"Select the places you would like to visit below."}
          places={pickedPlaces}
          onSelectPlace={handleStartRemovePlace}
        />
        <Places
          title="Available Places"
          places={AVAILABLE_PLACES}
          onSelectPlace={handleSelectPlace}
        />
      </main>
    </>
  );
}
```

Here getting the current location will happen when the component is loaded although it won't do anything to it.

To make it do something, we need a state that will be passed to the `Places` component and will be shown in the sorted order based on distance.

```js
function App() {
  // Code...
  const [avaialblePlaces, setAvailablePlaces] = useState([]);

  navigator.geolocation.getCurrentPosition((position) => {
    const sortedPlaces = sortPlacesByDistance(
      AVAILABLE_PLACES,
      position.coords.latitude,
      position.coords.longitude
    );
    setAvailablePlaces(sortedPlaces);
  });

  // Code...

  return (
    <>
      Code...
      <main>
        Code...
        <Places
          title="Available Places"
          places={avaialblePlaces}
          onSelectPlace={handleSelectPlace}
        />
      </main>
    </>
  );
}
```

But, this code has a huge problem. Infinite loops of state change. When the first time state update method is called, the component is called and then again the same code line will run and try to update the state again and this will go on creating an infinite loop.

This is the reason we need to take care of side effects effectively to avoid such situations.

To do that, we need to use `useEffect` hook that will take care of this side effect problem.

```js
useEffect(() => {
  navigator.geolocation.getCurrentPosition((position) => {
    const sortedPlaces = sortPlacesByDistance(
      AVAILABLE_PLACES,
      position.coords.latitude,
      position.coords.longitude
    );
    setAvailablePlaces(sortedPlaces);
  });
}, []);
```

With `useEffect` hook, the function defined inside it will only run after all the code inside the component and has run and the JSX code is returned.

Only after this will the code inside the `useEffect` will run. But if that is the logic then you might wander, don't we have the same problem like before? Because we are going to update the state and `useEffect` will run again which essentially will create the same infinite loop. That is where the second argument comes into play. As you can see it is an `[]` empty array which says to react that the `useEffect` has no dependencies and because of that it does not need to run the code again after re-evaluating the component for the first time in the 1st `useEffect` call.

If there were some dependencies included in the `[]` then depending on their changes the `useEffect` hook will run the code inside it again. If you remove this dependencies array then the `useEffect` code will be re-executes again therefore introducing the same infinite loop problem.

Therefore react never re-executes the code inside it after it has done so for once.

### Not all side effects need `useEffect`

Not all side effects need to be handled by `useEffect`, remember to only use them when they are necessary otherwise it will just be an overhead as it is called after the component execution.

For example, if you need to store the selected places inside localstorage then you are also writing some side effect code.

```js
function handleSelectPlace(id) {
  setPickedPlaces((prevPickedPlaces) => {
    if (prevPickedPlaces.some((place) => place.id === id)) {
      return prevPickedPlaces;
    }
    const place = AVAILABLE_PLACES.find((place) => place.id === id);
    return [place, ...prevPickedPlaces];
  });

  const pickedIds = localStorage.getItem("selectedPlaces") || [];
  if (pickedIds.indexOf("id" > -1)) return;
  localStorage.setItem("selectedPlaces", JSON.jsonify([id, ...pickedIds]));
}
```

It is ok to define this side effect inside the `handleSelectPlace` function because it does not create any infinite loop and also it makes sense to use it here, as we only add the place when user clicks on the image.

### Another example of `useEffect`

Let's look at another example of `useEffect`.

Following is the code to `Modal` component, which exposes the `ref` to outside components so that they can open or close the modal using `open()` or `close()` method on the `useRef` value passed to this `Modal` component.

```js
import { forwardRef, useImperativeHandle, useRef } from "react";
import { createPortal } from "react-dom";

const Modal = forwardRef(function Modal({ children }, ref) {
  const dialog = useRef();

  useImperativeHandle(ref, () => {
    return {
      open: () => {
        dialog.current.showModal();
      },
      close: () => {
        dialog.current.close();
      },
    };
  });

  return createPortal(
    <dialog className="modal" ref={dialog}>
      {children}
    </dialog>,
    document.getElementById("modal")
  );
});

export default Modal;
```

We can follow another approach for this.

Instead of calling the `modal.current.open()` or `modal.current.close()`, we can use a state `modalIsOpen` to declare whether the modal should be open or not.

Then, in the `Modal` component we can use,

```js
import { useRef } from "react";
import { createPortal } from "react-dom";

function Modal({ open, children }) {
  const dialog = useRef();

  return createPortal(
    <dialog className="modal" ref={dialog} open={open}>
      {children}
    </dialog>,
    document.getElementById("modal")
  );
}

export default Modal;
```

But the problem with this approach is, we are not getting the backdrop in the modal, as we are directly setting the `open` prop for `dialog`

We can think of an conditional approach where we open/close the modal depending on the `open` value.

```js
import { useRef, useEffect } from "react";
import { createPortal } from "react-dom";

function Modal({ open, children }) {
  const dialog = useRef();

  if (open) {
    dialog.current.showModal();
  } else {
    dialog.current.close();
  }

  return createPortal(
    <dialog className="modal" ref={dialog}>
      {children}
    </dialog>,
    document.getElementById("modal")
  );
}

export default Modal;
```

But this code fails!

The reason is, at first when the component loads, `open` is `false` and the `else` statement run.

BUT!

`dialog` ref is still not attached to the `dialog` component, so it contains `null` value now. Calling `close()` will create an error.

Here comes the `useEffect!

The reason `useEffect` makes sense in this case, is it helps us synchronize the states or props of the component with the DOM API.

Instead of directly setting the `open` prop, we can call the `showModal()` or `close()` on the `dialog` ref based on the value of the `open` prop coming to `Modal`.

```js
import { useRef, useEffect } from "react";
import { createPortal } from "react-dom";

function Modal({ open, children }) {
  const dialog = useRef();

  useEffect(() => {
    if (open) {
      dialog.current.showModal();
    } else {
      dialog.current.close();
    }
  }, [open]);

  return createPortal(
    <dialog className="modal" ref={dialog}>
      {children}
    </dialog>,
    document.getElementById("modal")
  );
}

export default Modal;
```

### Effect dependencies

Any value that makes the component to render again, if used in the `useEffect` block is called effect dependency.

Effect dependencies are mainly states/props because those are the values that make the component render.

Objects that are built-in from the browser and refs are not considered dependencies as they do not make the component to render again.

`useEffect` only considers the value as a dependency if it makes the component execute again, and why is that the case?

The `useEffect` should execute again if the component renders again, which means if one of it's dependencies change.

```js
useEffect(() => {
  if (open) {
    dialog.current.showModal();
  } else {
    dialog.current.close();
  }
}, [open]);
```

If you kept the dependencies array `[]`, then the `useEffect` will never run when the `open` value changes.

Which means that the modal will not show on the UI, as we are not running the `useEffect` code again.

But if you include the `open` prop in the dependencies array, the `useEffect` will run whenever the `open` prop changes which will check whether to open the modal or not.

### Another problem that useEffect can fix

Let's assume that when you click on any of the selected places, you want to automatically remove the place after 3 seconds even if the yes is not pressed.

To do that, you will use one of the brower functions `setTimeout` and pass a callback function as it's first argument and time in miliseconds as the second argument.

```js
export default function DeleteConfirmation({ onConfirm, onCancel }) {
  console.log("TIMER SET");
  setTimeout(() => {
    onConfirm();
  }, 3000);

  return (
    <div id="delete-confirmation">
      <h2>Are you sure?</h2>
      <p>Do you really want to remove this place?</p>
      <div id="confirmation-actions">
        <button onClick={onCancel} className="button-text">
          No
        </button>
        <button onClick={onConfirm} className="button">
          Yes
        </button>
      </div>
    </div>
  );
}
```

But this code has a problem - the `DeleteConfirmation` component is rendered on every app rendering, which means everytime the timer will be set and after 3 secs the timer will be out.

So to avoid this issue, we can conditionally choose to display this component.

```js
import { useRef, useEffect } from "react";
import { createPortal } from "react-dom";

function Modal({ open, onClose, children }) {
  // code...

  return createPortal(
    <dialog className="modal" ref={dialog} onClose={onClose}>
      {open ? children : null}
    </dialog>,
    document.getElementById("modal")
  );
}

export default Modal;
```

We are showing the `children` conditionally depending on whether the modal is open or not.

But even doing that, we are stuck with one major issue, and that is - when you try to delete any place but press "No", still the place will be deleted.

It is because the `setTimeout` is called when the `DeleteConfirmation` component renders.

We can use `useEffect` to get out of this issue. As this `setTimeOut` is not doing anything to render the JSX code. But, why do we need `useEffect` here?
Here are the following reasons we have been `useEffect` till now -

1. We had a `dialog` ref that was not connected yet, and we wanted to use that ref to open and close modal, which is considered a side-effect.
2. We used `useEffect` when we are using the location API to get the location and set the places according to their distance from current place. In this case it could cause infinite loop otherwise so we used `useEffect` to take care of the side-effect.

Just to be clear, we do not need the `useEffect` to define the `setTimeout` as it does not create any infinite loop nor is this a situation with ref where we needed to connect first. We need the `useEffect` to make sure we clean the `setTimeout` when the `DeleteConfirmation` component is removed from the DOM.

It is called clean-up function of `useEffect`. Clean-up function runs in 2 cases -

1. When the `useEffect` code is run again. Before the `useEffect` runs again, the clean-up function runs.
2. When the component dismounts from the DOM, which is the scenario here. So, before the component function is removed from the DOM.

```js
import { useEffect } from "react";

export default function DeleteConfirmation({ onConfirm, onCancel }) {
  useEffect(() => {
    console.log("TIMER SET");
    const timer = setTimeout(() => {
      onConfirm();
    }, 3000);

    // Cleanup function
    return () => {
      clearTimeout(timer);
    };
  }, []);

  return (
    <div id="delete-confirmation">
      <h2>Are you sure?</h2>
      <p>Do you really want to remove this place?</p>
      <div id="confirmation-actions">
        <button onClick={onCancel} className="button-text">
          No
        </button>
        <button onClick={onConfirm} className="button">
          Yes
        </button>
      </div>
    </div>
  );
}
```

Another important point to note - the cleanup function does not run before the first `useEffect` execution. It only runs before the subsequent `useEffect` executions and as mentioned after the component dismounts.

### Why Objects and Functions should not be included in dependency array

In Javascript, functions are also objects.

If we assign the object with some value and create another object with the same value, try to check their equality, it will be `false`.

Which means 2 objects with same value are not treated as equal.

Now, in react whenever the `App` component is rendered, the object is reassigned and when `useEffect` checks the dependency array and sees that the object contains a new value (though the same), it will run the `useEffect` even though the object value did not change.

```js
useEffect(() => {
  console.log("TIMER SET");
  const timer = setTimeout(() => {
    onConfirm();
  }, 3000);

  return () => {
    console.log("TIMER CLEARED");
    clearTimeout(timer);
  };
}, [onConfirm]);
```

But, in this example it will not cause infinite loop, as we are removing the `DeleteConfirmation` component when we confirm the deletion or keep it ideal for 3 secs.

```js
function handleRemovePlace() {
  setPickedPlaces((prevPickedPlaces) =>
    prevPickedPlaces.filter((place) => place.id !== selectedPlace.current)
  );
  const pickedIds = JSON.parse(localStorage.getItem("selectedPlaces")) || [];
  const pickedPlaces = pickedIds.filter((id) => id !== selectedPlace.current);
  localStorage.setItem("selectedPlaces", JSON.stringify([...pickedPlaces]));
  setModalIsOpen(false); // Here setting the modalIsOpen to false, we are dismounting the `DeleteConfirmation` component
}
```

But, if we remove this line, then an infinite loop will be created.

There is a hook that solves this issue though.

### useCallback hook

`useCallback` hook creates a function that will not be changed when the `App` component is rendered again.

It returns a function which stays in the store of react and only changes when any of the `useCallback` dependencies change.

```js
const handleRemovePlace = useCallback(function handleRemovePlace() {
  setPickedPlaces((prevPickedPlaces) =>
    prevPickedPlaces.filter((place) => place.id !== selectedPlace.current)
  );
  const pickedIds = JSON.parse(localStorage.getItem("selectedPlaces")) || [];
  const pickedPlaces = pickedIds.filter((id) => id !== selectedPlace.current);
  localStorage.setItem("selectedPlaces", JSON.stringify([...pickedPlaces]));
  setModalIsOpen(false);
}, []);
```

In this example, we do not need any dependencies in the dependency array as the function is, setting a state and using localstorage, which do not need any dependency.

### Another example of using cleanup function

Till now, what we have done shows the user a modal when they click on any place and if they don't do anything within 3 secs, the place will be deleted.

But, the problem here is, user might not understand what happened...

So to prevent that, we might want to show them a progress bar to show how much time is remaining.

In HTML, we have a built-in element called `progress` to show a progress bar.

This element takes 2 important props - `value` and `max`. `value` show show progress and `max` to let the browser decide the progress change based on the highest value.

```js
<progress value={remainingTime} max={TIMER} />
```

To know how much time is remaining, we use a state `remainingTime` set to `TIMER` which is `3s`.

```js
const [remainingTime, setRemainingTime] = useState(TIMER);
```

Then we use browser in-built function `setInterval` to update `remainingTime` by 10ms in every 10ms interval.

```js
setInterval(() => {
  setRemainingTime((prevTime) => prevTime - 10);
}, 10);
```

The `setInterval` runds the callback every 10ms, which renders the component again as we are setting a state `remainingTime`.

But the problem here is, we are not clearing this interval when the modal is closed, which creates an infinite loop for the whole component.

We already know the solution to this problem, defining the `setInterval` inside `useEffect` and cleaning the interval with a cleanup function.

### useEffect optimizations

As for the progress bar that you are seeing, we can optimize it a bit to reduce unnecessary computations and rendering of components.

We can seperate the progress part inside a `ProgressBar` component, and do the remaining time calculation there to stop rendering all the components and doing all other computations.

_ProgressBar.jsx_

```js
import { useState, useEffect } from "react";

export function ProgressBar({ timer }) {
  const [remainingTime, setRemainingTime] = useState(timer);

  useEffect(() => {
    console.log("INTERVAL");
    const interval = setInterval(() => {
      setRemainingTime((prevTime) => prevTime - 10);
    }, 10);

    return () => {
      clearInterval(interval);
    };
  }, []);

  return <progress value={remainingTime} max={timer} />;
}
```

_DeleteConfirmation.jsx_

```js
import { useEffect } from "react";
import { ProgressBar } from "./ProgressBar.jsx";

TIMER = 3000;

export default function DeleteConfirmation({ onConfirm, onCancel }) {
  // code...

  return (
    <div id="delete-confirmation">
      // Code...
      <ProgressBar timer={TIMER} />
    </div>
  );
}
```

## Testing React Apps

### What is "Testing"?

We have been doing testing since we started writing react code, by looking at the browser and checking whether that is the result we wanted and revisiting the code again if necessary.

This is manual testing. And it is very important as what you see will be what your user's also see. And it is important to optimize what users experience and check they don't face any issue in the process.

But manual testing is error-prone, as they just test one part of the application and not the entire application after making a particular change in the code base. This is not a good thing as changing one component and building new features might break some other features/components that you may not manually test. Also it is very hard to check all possible scenarios and combinations.

That is why there is another type of testing called automated testing, it is not a replacement for manual testing which is important. In automated testing, you write extra code that automatically tests your code.

With automated testing, you test the individual building block of your app. But, that also allows you test every building block in your app and checks all combinations and scenarios for all other blocks along with the individual block.

### Types of automated testing

There are 3 types of automated testing -

1. Unit Tests
2. Integration Tests
3. End-End(E2E) Tests

**Unit Tests**

Unit tests are the most important tests when doing automated tests as they test the individual building blocks that make up the application. It tests the blocks in isolations like testing some functions or testing some components which independently from the other components.

Projects typically contain a lot of unit tests like dozens or hundreds sometimes.

It is the most common/important kind of tests. The concept is, if individual units are working properly then the overall application should also work fine.

**Integration Tests**

Integration tests are to check some compoenents are working as expected with each other.

You don't have as many intgration tests as you have unit tests, often times a couple of integration tests that are also part of unit tests.

**End-End Tests**

E2E tests are basically testing complete scenarios / user flows in your app, as the user might experience when going through the application.

It is replicating the entire user experience when he is trying to do something with the application. It is similar to manual testing as it also tries to optimize the user experience overall.

It is important but also be done by manual testing.

### What and how to test?

It is important to be clear before testing anything to know "what" you are testing and "how" you are going to test it.

As mentioned, for unit tests you will be testing the smallest building blocks that make up your app. As smallest building blocks has fewer scenarios when they fail or success than larger blocks where there could a large no of scenarios where they may fail or succeed.

When it comes to "how", you have to test success and error cases for every unit test, and also rare (but possible) scenarios and their results.

## Required tools and set up

When doing unit testing, you need to have some extra set up to do the testing.

Typically there are 2 things involved when testing a react app -

- Tool to run the tests and assert the results
- Tool for "simulating" (rendering) the React app / component

For the first part, we have `Jest` (Not the only one but a popular choice) and for the second part, we have `React Testing Library`.

Fortunately, these 2 tools come along with any react app that is built using "creat-react-app". If you look at the `package.json` file of any new react project, you will see these tools in the `dependencies`.

```js
{
  ...
  "dependencies": {
    "@testing-library/jest-dom": "^5.11.6",
    "@testing-library/react": "^11.2.2",
    "@testing-library/user-event": "^12.5.0",
    ...
  },
  ...
}
```

## Running tests in React

Normally the default setup of the react app comes with `App.test.js` which is the file where all tests are written, and then `setUpTests.js` which set ups the whole testing environment.

The setup file should be as it is, what we need to look at is the `App.test.js` file which contains the tests.

**App.test.js**

```js
import { render, screen } from "@testing-library/react";
import App from "./App";

test("renders learn react link", () => {
  render(<App />);
  const linkElement = screen.getByText(/learn react/i);
  expect(linkElement).toBeInTheDocument();
});
```

We are writing the tests inside the `test` function which takes the name of the test and an anonymous function that explains what the test is.

We are using `render` function to render the `<App />` component and then using `screen` to get a text having `learn react` string (case insensitive - i) searching by regex expression.

Simply, we are tring to check whether the string `learn react` exists in the application interface or not which we can see in the `App.jsx` file.

```js
import logo from "./logo.svg";
import "./App.css";

function App() {
  return (
    <div className="App">
      <header className="App-header">
        <img src={logo} className="App-logo" alt="logo" />
        <p>
          Edit <code>src/App.js</code> and save to reload.
        </p>
        <a
          className="App-link"
          href="https://reactjs.org"
          target="_blank"
          rel="noopener noreferrer"
        >
          Learn React
        </a>
      </header>
    </div>
  );
}

export default App;
```

To run the tests, you need to go to terminal and write `npm test`.

After this you will see a result like the following-

```sh
 PASS  src/App.test.js
  √ renders learn react link (43 ms)

Test Suites: 1 passed, 1 total
Tests:       1 passed, 1 total
Snapshots:   0 total
Time:        2.83 s
Ran all test suites related to changed files.
```

## Writing your own tests

Let's create a `Greeting` component which has the following code -

```js
const Greeting = () => {
  return (
    <div>
      <h2>Hello World!</h2>
      <p>Welcome to this React Learning Resource.</p>
    </div>
  );
};

export default Greeting;
```

To test the above component, you need to write a file that contains the test code. You can write this testing code at `App.test.js` also, but it recommended to keep the test code as close as possible to the blocks you are testing.

So, we will create a `Greeting.test.js` file beside the `Greeting.jsx` file. The test file will contain the following code -

```js
import { render, screen } from "@testing-library/react";
import Greeting from "./Greeting.jsx";

test("rendering hello world in greeting component", () => {
  // Arrange
  render(<Greeting />);

  // Act
  // ...

  // Assert
  const element = screen.getByText("Hello World!");
  expect(element).toBeInTheDocument();
});
```

The test is checking whether the sentence `Hello World!` is present in the "virtual DOM" (coming from `render` function).

Since it is part of the component, it will pass the test.

```sh
 PASS  src/components/Greeting.test.js
  √ rendering hello world in greeting component (29 ms)

Test Suites: 1 passed, 1 total
Tests:       1 passed, 1 total
Snapshots:   0 total
Time:        2.587 s, estimated 3 s
Ran all test suites related to changed files.
```

To write any tests, you need to write three "A"s -

1. Arrange: Set up the test data, test conditions and test environment
2. Act: Run logic that should be tested(e.g. execute function)
3. Assert: Compare execution results with the expected results

## Working with Forms and User Inputs

### What's so difficult about forms?

When talking about forms, we talk about the in-built HTML `form` element that contains `input`s and `label`s inside them.

But, what the user wants with the form?

1. Form Submission: You want to submit the form data to somewhere so that you can do something with them. You might want to save them to database or process them in some way.
2. Input Validation: You wanna validate the input values of the form, and see whether they follow the rules specified for each field. You have to show errors if incorrect data has been provided.

Handling submissions can be relatively simplere, as we can use `states` (two-way binding), `refs`, or via `FormData` provided by the in-built browser.

The difficult part is validating the fields in the form, as providing a good user experience could be difficult -

1. You may validate on every keystroke - but the error may be shown too early.
2. You may validate when lost focus - but the error may be shown too late.
3. You can validate the form after the submission - but again the error is shown late.

### Handling Form Submission

When submitting the form, we need to keep in mind some factors.

The buttons inside form has by defauly `type=submit`, which means when they are clicked, the form will be submitted and the page will be reloaded. When form is submitted, their data goes to the server running on the same port, in this case react server.

This is not a server that has any code to handle submissions, as it is just a frontend code. So, many times you would want to prevent the form from submitting.

```js
export default function Login() {
  function handleFormSubmit(event) {
    event.preventDefault();
    console.log("Submitted!");
  }

  return (
    <form onSubmit={handleFormSubmit}>
      <h2>Login</h2>

      <div className="control-row">
        <div className="control no-margin">
          <label htmlFor="email">Email</label>
          <input id="email" type="email" name="email" />
        </div>

        <div className="control no-margin">
          <label htmlFor="password">Password</label>
          <input id="password" type="password" name="password" />
        </div>
      </div>

      <p className="form-actions">
        <button className="button button-flat">Reset</button>
        <button className="button">Login</button>
      </p>
    </form>
  );
}
```

In case of form, the best way to handle submit button clicks is to add the event handler at `form` element by setting the `onSubmit` property with the handler function.

This way, you get access to `event` which has `preventDefault()` method to stop the page reloading when clicked any `type=submit` button inside the form.

Often you would like to send the data of this form, to the standalone backend. For that we need to get the form data.

```js
import { useState } from "react";

export default function Login() {
  const [enteredValues, setEnteredValues] = useState({
    email: "",
    password: "",
  });

  function handleInputChange(identifier, value) {
    setEnteredValues((prevValues) => ({ ...prevValues, [identifier]: value }));
  }

  function handleFormSubmit(event) {
    event.preventDefault();
    console.log(enteredValues);
  }

  return (
    <form onSubmit={handleFormSubmit}>
      <h2>Login</h2>

      <div className="control-row">
        <div className="control no-margin">
          <label htmlFor="email">Email</label>
          <input
            id="email"
            type="email"
            name="email"
            onChange={(e) => handleInputChange("email", e.target.value)}
            value={enteredValues.email}
          />
        </div>

        <div className="control no-margin">
          <label htmlFor="password">Password</label>
          <input
            id="password"
            type="password"
            name="password"
            onChange={(e) => handleInputChange("password", e.target.value)}
            value={enteredValues.password}
          />
        </div>
      </div>

      <p className="form-actions">
        <button className="button button-flat">Reset</button>
        <button className="button">Login</button>
      </p>
    </form>
  );
}
```

The first option is definitely to use `useState` from react. Though you might want to create only one state as managing lots of states for each input field could be very tedious.

So, you will create a state containing an object `{email: "", password: ""}` that has the field ids as it's property and thier initial value is empty.

Then attach the handler to `onChange` property of each input field and call the handler with the `indentifier` and `value` (coming from `e.target.value`).

Also set the `value` prop of each input field to show the updated value of each input field.

Another solution could be to use `refs`.

```js
import { useRef } from "react";

export default function Login() {
  const email = useRef();
  const password = useRef();

  function handleFormSubmit(event) {
    event.preventDefault();

    const enteredEmail = email.current.value;
    const enteredPassword = password.current.value;

    console.log(enteredEmail, enteredPassword);

    email.current.value = "";
    password.current.value = "";
  }

  return (
    <form onSubmit={handleFormSubmit}>
      <h2>Login</h2>

      <div className="control-row">
        <div className="control no-margin">
          <label htmlFor="email">Email</label>
          <input id="email" type="email" name="email" ref={email} />
        </div>

        <div className="control no-margin">
          <label htmlFor="password">Password</label>
          <input id="password" type="password" name="password" ref={password} />
        </div>
      </div>

      <p className="form-actions">
        <button className="button button-flat">Reset</button>
        <button className="button">Login</button>
      </p>
    </form>
  );
}
```

First, we can initialize the refs for email and password fields as `email` & `password`, then we can pass this refs to the respective input fields as `ref` prop value.

Now, a connection has been established so we can directly get the values of the input fields from this `ref`s.

Like the example, you can use `email.current.value` or `password.current.value` to get the value of the input fields. Every input field has the `value` prop so we can access that in the ref.

At the end, you also need to reset the values of the form fields, by setting the ref values to empty string.

The drawbacks of this method is -

1. We are directly manipulating the DOM elements by setting their values.
2. It would become a very lengthy work if there were more form elements.

As an alternate way, we are going to discuss about another method `FormData`, an in-built method from browser to get the form data in an easy way.

```js
export default function Signup() {
  function handleSubmit(event) {
    event.preventDefault();

    const fd = new FormData(event.target);
    const enteredFormData = Object.fromEntries(fd.entries());
    console.log(enteredFormData);
  }

  return (
    <form onSubmit={handleSubmit}>
      <h2>Welcome on board!</h2>
      <p>We just need a little bit of data from you to get you started 🚀</p>

      <div className="control">
        <label htmlFor="email">Email</label>
        <input id="email" type="email" name="email" />
      </div>

      <div className="control-row">
        <div className="control">
          <label htmlFor="password">Password</label>
          <input id="password" type="password" name="password" />
        </div>

        <div className="control">
          <label htmlFor="confirm-password">Confirm Password</label>
          <input
            id="confirm-password"
            type="password"
            name="confirm-password"
          />
        </div>
      </div>

      <hr />

      <div className="control-row">
        <div className="control">
          <label htmlFor="first-name">First Name</label>
          <input type="text" id="first-name" name="first-name" />
        </div>

        <div className="control">
          <label htmlFor="last-name">Last Name</label>
          <input type="text" id="last-name" name="last-name" />
        </div>
      </div>

      <div className="control">
        <label htmlFor="phone">What best describes your role?</label>
        <select id="role" name="role">
          <option value="student">Student</option>
          <option value="teacher">Teacher</option>
          <option value="employee">Employee</option>
          <option value="founder">Founder</option>
          <option value="other">Other</option>
        </select>
      </div>

      <fieldset>
        <legend>How did you find us?</legend>
        <div className="control">
          <input
            type="checkbox"
            id="google"
            name="acquisition"
            value="google"
          />
          <label htmlFor="google">Google</label>
        </div>

        <div className="control">
          <input
            type="checkbox"
            id="friend"
            name="acquisition"
            value="friend"
          />
          <label htmlFor="friend">Referred by friend</label>
        </div>

        <div className="control">
          <input type="checkbox" id="other" name="acquisition" value="other" />
          <label htmlFor="other">Other</label>
        </div>
      </fieldset>

      <div className="control">
        <label htmlFor="terms-and-conditions">
          <input type="checkbox" id="terms-and-conditions" name="terms" />I
          agree to the terms and conditions
        </label>
      </div>

      <p className="form-actions">
        <button type="reset" className="button button-flat">
          Reset
        </button>
        <button type="submit" className="button">
          Sign up
        </button>
      </p>
    </form>
  );
}
```

As with this form, there are multiple fields present in the `Signup` form. Using `ref`s or `state`s could become tedious task. Instead it would be better to use `FormData` objects to get access to the form data entered in the form.

Note, the `FormData()` takes the form value as an argument, here that is `event.target`.

We can create a `FormData` instance `fd` and use the `entries()` to get `FormData Iterator`. We can access the values of this iterator by using `Object.fromEntries()` method.

Output:

```sh
{
    "email": "test@example.com",
    "password": "2812918",
    "confirm-password": "128918291",
    "first-name": "Arup",
    "last-name": "Jana",
    "role": "student",
    "terms": "on"
}
```

But, this won't give us some of the input field values that are inside another field, e.g. it does not give the input field values inside `fieldset`.

To get the field values inside the `fieldset`, we need to target them by their `name` prop and get all of their values by using `fd.getAll()`.

`getAll` takes the `name` of the fields as the argument.

```js
export default function Signup() {
  function handleSubmit(event) {
    event.preventDefault();

    const fd = new FormData(event.target);

    // This misses the inner input field values (inside fieldset)
    const enteredFormData = Object.fromEntries(fd.entries());

    // Explicitely getting all values as an array
    const acquisitionData = fd.getAll("acquisition");
    enteredFormData.acquisition = acquisitionData;

    console.log(enteredFormData);
  }

  return (
    <form onSubmit={handleSubmit}>
      <h2>Welcome on board!</h2>
      <p>We just need a little bit of data from you to get you started 🚀</p>

      <div className="control">
        <label htmlFor="email">Email</label>
        <input id="email" type="email" name="email" />
      </div>

      <div className="control-row">
        <div className="control">
          <label htmlFor="password">Password</label>
          <input id="password" type="password" name="password" />
        </div>

        <div className="control">
          <label htmlFor="confirm-password">Confirm Password</label>
          <input
            id="confirm-password"
            type="password"
            name="confirm-password"
          />
        </div>
      </div>

      <hr />

      <div className="control-row">
        <div className="control">
          <label htmlFor="first-name">First Name</label>
          <input type="text" id="first-name" name="first-name" />
        </div>

        <div className="control">
          <label htmlFor="last-name">Last Name</label>
          <input type="text" id="last-name" name="last-name" />
        </div>
      </div>

      <div className="control">
        <label htmlFor="phone">What best describes your role?</label>
        <select id="role" name="role">
          <option value="student">Student</option>
          <option value="teacher">Teacher</option>
          <option value="employee">Employee</option>
          <option value="founder">Founder</option>
          <option value="other">Other</option>
        </select>
      </div>

      <fieldset>
        <legend>How did you find us?</legend>
        <div className="control">
          <input
            type="checkbox"
            id="google"
            name="acquisition"
            value="google"
          />
          <label htmlFor="google">Google</label>
        </div>

        <div className="control">
          <input
            type="checkbox"
            id="friend"
            name="acquisition"
            value="friend"
          />
          <label htmlFor="friend">Referred by friend</label>
        </div>

        <div className="control">
          <input type="checkbox" id="other" name="acquisition" value="other" />
          <label htmlFor="other">Other</label>
        </div>
      </fieldset>

      <div className="control">
        <label htmlFor="terms-and-conditions">
          <input type="checkbox" id="terms-and-conditions" name="terms" />I
          agree to the terms and conditions
        </label>
      </div>

      <p className="form-actions">
        <button type="reset" className="button button-flat">
          Reset
        </button>
        <button type="submit" className="button">
          Sign up
        </button>
      </p>
    </form>
  );
}
```

Before we dive into validating the form inputs, we must understand how to reset the form manually.

We can use a button prop `type=reset` to set the button as a reset button, and whenever that button is clicked we will be able to reset the form.

If we are using states then we can use the initial values of the state to reset the values-

```js
setEnteredValues({
  email: "",
  password: "",
});
```

or if we are using refs, then we can directly set the values as empty string-

```js
email.current.value = "";
password.current.value = "";
```

But we wanna manually reset the form with code, then we can do that by -

```js
function handleSubmit(event) {
  // ...

  event.target.reset();
}
```

### Validating form inputs

There are multiple ways to validate the form inputs, the first method would be to validate the fields on every keystroke.

For that, we need to follow the state approach to make sure that we have access to the current value of input as they are changing.

For example, we can compute the validity of email based on the current state value of the email, and then show an error message if it is invalid.

```js
import { useState } from "react";

export default function Login() {
  const [enteredValues, setEnteredValues] = useState({
    email: "",
    password: "",
  });

  const emailIsInvalid = !enteredValues.email.includes("@");

  function handleFormSubmit(event) {
    event.preventDefault();

    console.log(enteredValues);
  }

  function handleInputChange(value, id) {
    setEnteredValues((prev) => ({ ...prev, [id]: value }));
  }

  return (
    <form onSubmit={handleFormSubmit}>
      <h2>Login</h2>

      <div className="control-row">
        <div className="control no-margin">
          <label htmlFor="email">Email</label>
          <input
            id="email"
            type="email"
            name="email"
            value={enteredValues.email}
            onChange={(e) => handleInputChange(e.target.value, "email")}
          />
          <div className="control-error">
            {emailIsInvalid && <p>Please enter a valid email address.</p>}
          </div>
        </div>

        <div className="control no-margin">
          <label htmlFor="password">Password</label>
          <input
            id="password"
            type="password"
            name="password"
            value={enteredValues.password}
            onChange={(e) => handleInputChange(e.target.value, "password")}
          />
        </div>
      </div>

      <p className="form-actions">
        <button className="button button-flat">Reset</button>
        <button className="button">Login</button>
      </p>
    </form>
  );
}
```

Here, you can see we used one computed variable `emailIsInvalid` by searching whether `@` is present.

But the problem is even before user puts something, the error is shown.

So to resolve that issue, we can change the condition a little bit with the following -

```js
const emailIsInvalid =
  enteredValues.email !== "" && !enteredValues.email.includes("@");
```

This will handle the problem, but there are still other problems -

1. After putting something, if the user cleans the field, they don't see any error message. Because we are always ignoring the empty field scenario with the above condition.
2. Users do not get the opportunity to put the field properly, and they are shown error message e.g. `test...` which is not good.

For the mentioned reasons, we need to make sure to show the error after user has properly filled the field.

You can validate user input when the input losses focus (`blur`).

We can pass method to `onBlur` prop to listen to blur event so that whenever the input losses focus, we check whether the email is valid or not, and depending on the validity we may show them the error message.

```js
import { useState } from "react";

export default function Login() {
  const [enteredValues, setEnteredValues] = useState({
    email: "",
    password: "",
  });
  // whether input fields are blurred after being edited
  const [didEdit, setDidEdit] = useState({
    email: false,
    password: false,
  });

  const emailIsInvalid = didEdit.email && !enteredValues.email.includes("@");

  function handleFormSubmit(event) {
    event.preventDefault();

    console.log(enteredValues);
  }

  function handleInputChange(value, id) {
    setEnteredValues((prev) => ({ ...prev, [id]: value }));

    // Remove the error message when user is typing
    setDidEdit((prevEdit) => ({
      ...prevEdit,
      [id]: false,
    }));
  }

  function handleInputBlur(id) {
    setDidEdit((prevEdit) => ({
      ...prevEdit,
      [id]: true,
    }));
  }

  return (
    <form onSubmit={handleFormSubmit}>
      <h2>Login</h2>

      <div className="control-row">
        <div className="control no-margin">
          <label htmlFor="email">Email</label>
          <input
            id="email"
            type="email"
            name="email"
            value={enteredValues.email}
            onBlur={() => handleInputBlur("email")}
            onChange={(e) => handleInputChange(e.target.value, "email")}
          />
          <div className="control-error">
            {emailIsInvalid && <p>Please enter a valid email address.</p>}
          </div>
        </div>

        <div className="control no-margin">
          <label htmlFor="password">Password</label>
          <input
            id="password"
            type="password"
            name="password"
            value={enteredValues.password}
            onChange={(e) => handleInputChange(e.target.value, "password")}
          />
        </div>
      </div>

      <p className="form-actions">
        <button className="button button-flat">Reset</button>
        <button className="button">Login</button>
      </p>
    </form>
  );
}
```

This is a better solution, make sure you add the following code snippet -

```js
// Remove the error message when user is typing
setDidEdit((prevEdit) => ({
  ...prevEdit,
  [id]: false,
}));
```

This resolves the following issue -

The error stays for too long. Means when the user start typing, the error is still there unless the user provides the correct syntax of email.

To avoid that we make sure when user start typing in the input, they do not get the error message until the input loss focus.

We have another method of showing the error at the time of submitting.

```js
import { useRef, useState } from "react";

export default function Login() {
  const [emailIsInvalid, setEmailIsInvalid] = useState(false);

  const email = useRef();
  const password = useRef();

  function handleFormSubmit(event) {
    event.preventDefault();

    const enteredEmail = email.current.value;
    const enteredPassword = password.current.value;

    const emailIsValid = enteredEmail.includes("@");
    if (!emailIsValid) {
      setEmailIsInvalid(true);
      return;
    }

    setEmailIsInvalid(false);
    console.log(enteredEmail, enteredPassword);

    email.current.value = "";
    password.current.value = "";
  }

  return (
    <form onSubmit={handleFormSubmit}>
      <h2>Login</h2>

      <div className="control-row">
        <div className="control no-margin">
          <label htmlFor="email">Email</label>
          <input id="email" type="email" name="email" ref={email} />
          <div className="control-error">
            {emailIsInvalid && <p>Please enter a valid email address.</p>}
          </div>
        </div>

        <div className="control no-margin">
          <label htmlFor="password">Password</label>
          <input id="password" type="password" name="password" ref={password} />
        </div>
      </div>

      <p className="form-actions">
        <button className="button button-flat">Reset</button>
        <button className="button">Login</button>
      </p>
    </form>
  );
}
```

Validate the values in the input field when they submit and return if the validation fails. Otherwise, proceed with any HTTP request.

```js
const emailIsValid = enteredEmail.includes("@");
if (!emailIsValid) {
  setEmailIsInvalid(true);
  return;
}

setEmailIsInvalid(false);
```

Usually when you implement the validation on lose focus, you also have to implement this validation method before submitting so that wrong data is not passed to the server.

We can also use the built-in validation provided by the browser to validate the input fields -

- `required`: Specifies whether a form field needs to be filled in before the form can be submitted.
- `minlength` and `maxlength`: Specifies the minimum and maximum length of textual data (strings).
- `min`, `max`, and `step`: Specifies the minimum and maximum values of numerical input types, and the increment, or step, for values, starting from the minimum.
- `type`: Specifies whether the data needs to be a number, an email address, or some other specific preset type.
- `pattern`: Specifies a regular expression that defines a pattern the entered data needs to follow.

For more information, you can refer to [this link](https://developer.mozilla.org/en-US/docs/Learn/Forms/Form_validation).

You might sometime want to combine the built-in and custom validation for advanced use cases.

For example, you need to validate whether the `confirm-password` is same as the `password`.

Then along with the built-in validation you have to also add custom check for matching the `password` and `confirm-password`.

```js
import { useState } from "react";

export default function Signup() {
  const [passwordNotMatched, setPasswordNotMatched] = useState();

  function handleSubmit(event) {
    event.preventDefault();

    const fd = new FormData(event.target);
    // This misses the inner input field values (inside fieldset)
    const enteredFormData = Object.fromEntries(fd.entries());

    // Explicitely getting all values as an array
    const acquisitionData = fd.getAll("acquisition");
    enteredFormData.acquisition = acquisitionData;

    if (enteredFormData.password !== enteredFormData["confirm-password"]) {
      setPasswordNotMatched(true);
      return;
    }

    console.log(enteredFormData);
  }

  return (
    <form onSubmit={handleSubmit}>
      <h2>Welcome on board!</h2>
      <p>We just need a little bit of data from you to get you started 🚀</p>

      <div className="control">
        <label htmlFor="email">Email</label>
        <input id="email" type="email" name="email" required />
      </div>

      <div className="control-row">
        <div className="control">
          <label htmlFor="password">Password</label>
          <input
            id="password"
            type="password"
            name="password"
            required
            minLength={8}
          />
        </div>

        <div className="control">
          <label htmlFor="confirm-password">Confirm Password</label>
          <input
            id="confirm-password"
            type="password"
            name="confirm-password"
            required
          />
          <div className="control-error">
            {passwordNotMatched && <p>Password does not match.</p>}
          </div>
        </div>
      </div>

      <hr />

      <div className="control-row">
        <div className="control">
          <label htmlFor="first-name">First Name</label>
          <input type="text" id="first-name" name="first-name" required />
        </div>

        <div className="control">
          <label htmlFor="last-name">Last Name</label>
          <input type="text" id="last-name" name="last-name" required />
        </div>
      </div>

      <div className="control">
        <label htmlFor="phone">What best describes your role?</label>
        <select id="role" name="role" required>
          <option value="student">Student</option>
          <option value="teacher">Teacher</option>
          <option value="employee">Employee</option>
          <option value="founder">Founder</option>
          <option value="other">Other</option>
        </select>
      </div>

      <fieldset>
        <legend>How did you find us?</legend>
        <div className="control">
          <input
            type="checkbox"
            id="google"
            name="acquisition"
            value="google"
          />
          <label htmlFor="google">Google</label>
        </div>

        <div className="control">
          <input
            type="checkbox"
            id="friend"
            name="acquisition"
            value="friend"
          />
          <label htmlFor="friend">Referred by friend</label>
        </div>

        <div className="control">
          <input type="checkbox" id="other" name="acquisition" value="other" />
          <label htmlFor="other">Other</label>
        </div>
      </fieldset>

      <div className="control">
        <label htmlFor="terms-and-conditions">
          <input
            type="checkbox"
            id="terms-and-conditions"
            name="terms"
            required
          />
          I agree to the terms and conditions
        </label>
      </div>

      <p className="form-actions">
        <button type="reset" className="button button-flat">
          Reset
        </button>
        <button type="submit" className="button">
          Sign up
        </button>
      </p>
    </form>
  );
}
```

### Making input and validation re-usable

And for this example, we have some repeatetive code that could be moved to a seperate component like `Input`.

_Input.jsx_

```js
export default function Input({ label, id, error, ...props }) {
  return (
    <div className="control no-margin">
      <label htmlFor={id}>{label}</label>
      <input id={id} {...props} />
      <div className="control-error">{error && <p>{error}</p>}</div>
    </div>
  );
}
```

We can also outsource the validation logics for email and password.

_utils/validation.js_

```js
export function isEmail(value) {
  return value.includes("@");
}

export function isNotEmpty(value) {
  return value.trim() !== "";
}

export function hasMinLength(value, minLength) {
  return value.length >= minLength;
}

export function isEqualsToOtherValue(value, otherValue) {
  return value === otherValue;
}
```

After the changes, the `StatefulLogin.jsx` looks like this -

```jsx
import { useState } from "react";
import Input from "./Input.jsx";
import {
  isEmail,
  isNotEmpty,
  hasMinLength,
  isEqualsToOtherValue,
} from "../util/validation.js";

export default function Login() {
  const [enteredValues, setEnteredValues] = useState({
    email: "",
    password: "",
  });
  // whether input fields are blurred after being edited
  const [didEdit, setDidEdit] = useState({
    email: false,
    password: false,
  });

  const emailIsInvalid =
    didEdit.email &&
    !isEmail(enteredValues.email) &&
    !isNotEmpty(enteredValues.email);
  const passwordIsInvalid =
    didEdit.password && !hasMinLength(enteredValues.password, 6);

  function handleFormSubmit(event) {
    event.preventDefault();

    console.log(enteredValues);
  }

  function handleInputChange(value, id) {
    setEnteredValues((prev) => ({ ...prev, [id]: value }));
    setDidEdit((prevEdit) => ({
      ...prevEdit,
      [id]: false,
    }));
  }

  function handleInputBlur(id) {
    setDidEdit((prevEdit) => ({
      ...prevEdit,
      [id]: true,
    }));
  }

  return (
    <form onSubmit={handleFormSubmit}>
      <h2>Login</h2>

      <div className="control-row">
        <Input
          label="Email"
          id="email"
          type="email"
          name="email"
          value={enteredValues.email}
          onBlur={() => handleInputBlur("email")}
          onChange={(e) => handleInputChange(e.target.value, "email")}
          error={emailIsInvalid && "Please enter a valid email."}
        />

        <Input
          label="Password"
          id="password"
          type="password"
          name="password"
          value={enteredValues.password}
          onBlur={() => handleInputBlur("password")}
          onChange={(e) => handleInputChange(e.target.value, "password")}
          error={passwordIsInvalid && "Please enter a valid password."}
        />
      </div>

      <p className="form-actions">
        <button className="button button-flat">Reset</button>
        <button className="button">Login</button>
      </p>
    </form>
  );
}
```

## Debugging React Apps

Debugging React apps is an essential part of development as it is natural to get errors when building applications.

While React error messages could be daunting, they are also very much clear on where to look for the error.

E.g. in this message, you can clearly see that the error is coming from the line number 8 and in character 16

![Error message](./images/error.png)

which appears to be this line -

```js
export default function Results({ input }) {
  const results = [];
  calculateInvestmentResults(input, results);
  const initialInvestment =
->  results[0].valueEndOfYear -
    results[0].interest -
    results[0].annualInvestment;

    // ...
}

```

It seems that `results[0]` is `undefined`, which can happen if the duration is less than 1.

```js
export function calculateInvestmentResults(
  { initialInvestment, annualInvestment, expectedReturn, duration },
  results
) {
  let investmentValue = initialInvestment;

  for (let i = 0; i < duration; i++) {
    const interestEarnedInYear = investmentValue * (expectedReturn / 100);
    investmentValue += interestEarnedInYear + annualInvestment;
    results.push({
      year: i + 1, // year identifier
      interest: interestEarnedInYear, // the amount of interest earned in this year
      valueEndOfYear: investmentValue, // investment value at end of year
      annualInvestment: annualInvestment, // investment added in this year
    });
  }
}
```

Now, we know where the problem can come from. So, to make sure that if `results` is empty we return something else, we can do something like the following -

```js
if (results.length === 0) {
  return <p className="center">Invalid input, please put a valid duration.</p>;
}

const initialInvestment =
  results[0].valueEndOfYear - results[0].interest - results[0].annualInvestment;
```

To check logical errors, we have to first think about from where the error could be coming.

As we understand the problem better, we can add some breakpoints in the code in the browser. Breakpoints will stop the execution when that line is going to be executed.

We also get buttons like `next step`, `move into block` and `move out of block`.

Another way to debug your application is by using react `StrictMode`. In this mode, every component is rendered twice which sometimes help you catch issues that might occur due to component re-rendering which won't be immediately caught at first.

## Sending HTTP Requests

Till now, we have been storing the data in our local machine and getting from there.

But, if we want to build an app that can communicate changes between multiple clients throughout the world, and know about the changes - we need to have a central server / database that keeps the data and all clients will know about the changes from the database / server.

We can't connect with database from our react app - because we can have security issues if we do that.

React code is run in the browser, and the user can look into the react code using the developer tools. If you use any database credentials of your database here, then your database will be compromised.

That's why we need a middle man - in this case the server.

The backend server will be the application that communicates with the database and provides the data to frontend. In this way, we can securely fetch data from backend without letting the user know where the data is coming from or how the data is coming.

Another reason is browser is not cabable of opening files. If there is any shared file that you need in your application, then it is not possible for browser to get that file.

The frontend and backend communicate using HTTP requests. And the good thing about having a backend is - user can send only requests that allowed. In other words, the URLs that the backend exposes to the frontend - also known as API endpoints.

### How NOT To Send HTTP Request

We can send HTTP request using `fetch`. If we use fetch and get the places data like following -

```js
import { useState } from "react";
import Places from "./Places.jsx";

export default function AvailablePlaces({ onSelectPlace }) {
  const [availablePlaces, setAvailablePlaces] = useState([]);

  fetch("http://localhost:3000/places")
    .then((response) => response.json())
    .then((res) => setAvailablePlaces(res.places));

  return (
    <Places
      title="Available Places"
      places={availablePlaces}
      fallbackText="No places available."
      onSelectPlace={onSelectPlace}
    />
  );
}
```

But the problem with this approach is - it will create an infinite loop.

Everytime the component renders, the fetch will be called and the `setAvailablePlaces` will render the component again, then the same thing will happen again again.

### Async / Await

If you want to fetch the data using `async` / `await`- you can create an async function inside the `useEffect` and call it.

```js
useEffect(() => {
  async function getPlaces() {
    const response = await fetch("http://localhost:3000/places");
    const resData = await response.json();
    setAvailablePlaces(resData.places);
  }

  getPlaces();
}, []);
```

### Error in HTTP requests

Sometimes the backend server may fail due to many reasons - network failure, server failure, incorrect backend code etc.

But, the frontend should be prepared to show the user what happened in case of error.

For that we need to check for the `fetch` block and see whether the response we got resulted in failure or not.

For that we can use `response.ok` to check whether the response is a `200`, `300` or `400`.

In case of success the value will be `true` otherwise `false`.

```js
import { useState } from "react";
import Places from "./Places.jsx";
import Error from "./Error.jsx";
import { useEffect } from "react";

export default function AvailablePlaces({ onSelectPlace }) {
  const [availablePlaces, setAvailablePlaces] = useState([]);
  const [isFetching, setIsFetching] = useState(false);
  const [error, setError] = useState();

  useEffect(() => {
    async function getPlaces() {
      setIsFetching(true);

      try {
        const response = await fetch("http://localhost:3000/places");
        const resData = await response.json();

        if (!response.ok) {
          throw new Error("Failed to fetch places");
        }

        setAvailablePlaces(resData.places);
      } catch (error) {
        setError({
          message:
            error.message ||
            "Failed to fetch the places, please try again later.",
        });
      }

      setIsFetching(false);
    }

    getPlaces();
  }, []);

  if (error) {
    return (
      <Error title="An Error Occured When Fetching" message={error.message} />
    );
  }

  return (
    <Places
      title="Available Places"
      places={availablePlaces}
      fallbackText="No places available."
      loadingText="Loading available places..."
      loading={isFetching}
      onSelectPlace={onSelectPlace}
    />
  );
}
```

### Optimistic Updating

You show spinners or some loading state when you are calling the backend server and you know it might take sometime.

But sometimes when you change something in the frontend and try to send the data to backend, you can handle it with 2 different ways.

You can send the data immediately after user changes something in the frontend - and then while the backend server process this data you show a loading state till the response comes.

Or, you can change the state of your frontend optimistically before sending the data and at the same time in the background send the data to backend server so that it can update the server data.

Looking at the following example where I am adding the selected place in the state `userPlaces` and then sending the data to backend -

```js
async function handleSelectPlace(selectedPlace) {
  setUserPlaces((prevPickedPlaces) => {
    if (!prevPickedPlaces) {
      prevPickedPlaces = [];
    }
    if (prevPickedPlaces.some((place) => place.id === selectedPlace.id)) {
      return prevPickedPlaces;
    }
    return [selectedPlace, ...prevPickedPlaces];
  });

  try {
    await updateSelectedPlaces([selectedPlace, ...userPlaces]);
  } catch (error) {
    // error...
  }
}
```

I am not showing any loading state - instead I changed the state of the frontend for optimal user experience and simultaneously sends data to backend.

In this case, if the backend server fails then we can move back to the previous state by doing this -

```js
try {
  await updateSelectedPlaces([selectedPlace, ...userPlaces]);
} catch (error) {
  setUserPlaces(userPlaces);
}
```

But suddenly reverting back to previous change might confuse the user. So, we should show an error message after reverting back to previous changes.

```js
import { useRef, useState, useCallback } from "react";

import Places from "./components/Places.jsx";
import ErrorMsg from "./components/Error.jsx";
import Modal from "./components/Modal.jsx";
import DeleteConfirmation from "./components/DeleteConfirmation.jsx";
import logoImg from "./assets/logo.png";
import AvailablePlaces from "./components/AvailablePlaces.jsx";
import { updateSelectedPlaces } from "./http.js";

function App() {
  // ...

  const [userPlaces, setUserPlaces] = useState([]);
  const [errorUpdating, setErrorUpdating] = useState();

  // ...

  async function handleSelectPlace(selectedPlace) {
    setUserPlaces((prevPickedPlaces) => {
      if (!prevPickedPlaces) {
        prevPickedPlaces = [];
      }
      if (prevPickedPlaces.some((place) => place.id === selectedPlace.id)) {
        return prevPickedPlaces;
      }
      return [selectedPlace, ...prevPickedPlaces];
    });

    try {
      await updateSelectedPlaces([selectedPlace, ...userPlaces]);
    } catch (error) {
      setUserPlaces(userPlaces);
      setErrorUpdating(error);
    }
  }

  // ...

  function handleError() {
    setErrorUpdating(null);
  }

  return (
    <>
      <Modal open={errorUpdating} onClose={handleError}>
        {errorUpdating && (
          <ErrorMsg
            title="Error updating user places"
            message={errorUpdating.message}
            onConfirm={handleError}
          />
        )}
      </Modal>
      // ...
    </>
  );
}

export default App;
```
