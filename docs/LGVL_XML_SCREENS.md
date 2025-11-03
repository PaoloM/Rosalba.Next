# LGVL XML SCREENS

TBD

## Requirements

- lvgl v9.4+
- ESP32

## Elements

- **screen**:

## Attribute Definitions

### Common Widget Attributes

- **id**: Optional unique identifier for the widget, used for data binding and referencing in code.
- **x**: Horizontal position of the widget within its parent. Accepts pixels (e.g., `20`), or percentages (e.g., `50%`).
- **y**: Vertical position of the widget within its parent. Accepts pixels, or percentages.
- **width**: Width of the widget. Accepts pixels or percentages relative to parent.
- **height**: Height of the widget. Accepts pixels or percentages relative to parent.
- **color**: Foreground color (e.g., text color for labels, line color for lines). Format: `#RRGGBB`.
- **background-color**: Background fill color for the widget. Format: `#RRGGBB`.
- **border**: Border thickness in pixels.
- **border-color**: Color of the border. Format: `#RRGGBB`.
- **margin**: Space outside the widget, in pixels.
- **padding**: Space inside the widget, in pixels.
- **align**: Horizontal alignment within parent (`left`, `center`, `right`).
- **valign**: Vertical alignment within parent (`top`, `middle`, `bottom`).

### Container Attributes

- **panel**: Container for more widgets, does not enforce any layout
- **stackpanel**: Container that stacks child widgets in a defined direction. In a vertical stackpanel, if a child does not specify a width, it should default to 100% of the parent stackpanel's width. In a horizontal stackpanel, if a child does not specify a height, it should default to 100% of the parent stackpanel's height.
  - **orientation**: Layout direction for children (`horizontal` or `vertical`).

### Widget-Specific Attributes

#### Label
- **font**: Font family and size (e.g., `Montserrat_16`).
- **text**: Text content to display.

#### Line
- **x1**: Start point X coordinate (pixels or percentage).
- **y1**: Start point Y coordinate.
- **x2**: End point X coordinate.
- **y2**: End point Y coordinate.
- **stroke**: Line thickness (in pixels).

#### Image
- **src**: Path or resource name for the image file.

#### Circle
- **radius**: Radius of the circle (pixels or percentage).
- **fill-color**: Fill color of the circle. Format: `#RRGGBB`.
- **stroke**: Border thickness (in pixels).