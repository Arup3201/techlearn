# Learnings - Web Development

# Filter - drop shadow

On images, `box-shaow` does not work. So, if you want to make a shadow effect on images, you need to use `filter` property and set it to `drop-shadow(0 0 8px rgba(0,0,0,0.4))`

```css
img-element {
  filter: drop-shadow(0, 0, 8px, rgba(0, 0, 0, 0.4));
}
```

# Make an image header for a card

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
