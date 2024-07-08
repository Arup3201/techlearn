# Notes on Reast.JS - From Basics

## React Project Setup

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

## Import and Exports

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

## Components

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

## First Custom Component

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

## How components are handled by React?

As you already know components are written in JSX and they are unsupported in browser. Then how react handles them?

If you look at the source code of the page, then you will notice that no component is present there. Only the script import `index.jsx` is present.

Now, if you look at the code written in `index.jsx` file, you will see that I am importing the `App` from `App.jsx` file and then passing it to `render` method.

`ReactDOM` is the library that is responsible for converting this component code to browser compatible HTML, CSS and JS code. To do that I am rendering this component on root that is created by passing the `root` element created in the `index.html` file to the `create_root` method and rendering the components.

If `App` component has sub components inside it then React will go through each component in the `Component Tree` untill it finds built-in HTML DOM elements.

> That is also the reason why the component names should start with upper-case letters as they will collide with the built-in HTML DOM elements that start with lower-case letters.

## Using and outputing random dynamic values

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

## Component "props" concept

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

## Composition props

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

## Handling events in React

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

## Passing functions as values to props

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

## Passing custom arguments to event functions

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

## State (**useState**)

When you want to dyanmically change the contents of the react components you need to "tell" react app to reload so that it will execute the components and render them on UI again.

It is not possible by just changing a variable when an event happens and making it dynamically loading it in react component code.

Instead, you have to use `State` of a variable and "tell" react to render UI again because the state has changed.

To do that you have to use `useState` function from react. It is called a react hook.

React hooks start with `use` keyword. Here, the react hook that you will be using is the `useState` hook that

2 rules of hooks -

1. Only call hooks inside of component functions. It must not be called outside react components.
2. Only call hooks on the top level. They must not be called inside nested code statements.

## Showing conditional components

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

## CSS dynamic styling

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

## Output lists dynamically

JSX code also outputs list with JSX code, so we can do something like this to dynamically load lists -

```js
{
  CORE_CONCEPTS.map((coreConcept) => (
    <CoreConcepts key={coreConcept.title} {...coreConcept} />
  ));
}
```

This is much more efficient, scalable.
