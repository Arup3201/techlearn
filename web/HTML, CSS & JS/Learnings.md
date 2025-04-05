# Learnings - Web Development

## Filter - drop shadow

On images, `box-shaow` does not work. So, if you want to make a shadow effect on images, you need to use `filter` property and set it to `drop-shadow(0 0 8px rgba(0,0,0,0.4))`

```css
img-element {
  filter: drop-shadow(0, 0, 8px, rgba(0, 0, 0, 0.4));
}
```

## Make an image header for a card

I am making a card with an image as a header. To do that-

```css
.blog img {
  width: 100%;
  max-height: 70%;
  object-fit: cover;
}
```

`object-fit`: The object-fit CSS property sets how the content of a replaced element, such as an `<img>` or `<video>`, should be resized to fit its container. Values- contain, cover, fill, none, scale-down

`contain`
The replaced content is scaled to maintain its aspect ratio while fitting within the element's content box. The entire object is made to fill the box, while preserving its aspect ratio, so the object will be "letterboxed" if its aspect ratio does not match the aspect ratio of the box.

`cover`
The replaced content is sized to maintain its aspect ratio while filling the element's entire content box. If the object's aspect ratio does not match the aspect ratio of its box, then the object will be clipped to fit.

`fill`
The replaced content is sized to fill the element's content box. The entire object will completely fill the box. If the object's aspect ratio does not match the aspect ratio of its box, then the object will be stretched to fit.

`none`
The replaced content is not resized.

`scale-down`
The content is sized as if none or contain were specified, whichever would result in a smaller concrete object size.

## Grid Auto

To make automatic columns/rows withour specifying their numbers, you can use `grid-auto-flow` property.

```css
.grid-container {
  display: grid;
  grid-auto-flow: column;
}
```

Now, you can add any number of columns to this grid container.

To make gaps work, make sure that you do not fix their width (height in case of `grid-auto-flow: row;`).

```css
.grid-item {
  /* Other properties */
  /* width: 100%; X */
}
```

## JS Sort Function

The `sort()` method of `Array` instances sorts the elements of an array _in place_ and returns the reference to the same array, now sorted. The default sort order is ascending, built upon converting the elements into strings, then comparing their sequences of UTF-16 code units values.

**Syntax**

```js
sort();
sort(compareFn);
```

Parameters:

- compareFn (Optional):
  A function that determines the order of the elements. The function is called with the following arguments:

  - a: The first element for comparison. Will never be undefined.
  - b: The second element for comparison. Will never be undefined.

  It should return a number where:

  - A negative value indicates that `a` should come before `b`.
  - A positive value indicates that `a` should come after `b`.
  - Zero or `NaN` indicates that `a` and `b` are considered equal.
    To memorize this, remember that `(a, b) => a - b` sorts numbers in ascending order.
    If omitted, the array elements are converted to strings, then sorted according to each character's Unicode code point value.
