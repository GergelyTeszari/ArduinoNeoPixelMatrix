# Static image source files

The files in `assets/patterns/txt` are editable source snippets for the static
Arduino firmware. They are intentionally kept as `.txt` files so a picture can
be copied directly into the `colorMatrix` initializer without running a
conversion tool.

## File format

Each image contains 16 non-empty lines. Every line contains 16 expressions in
this form:

```cpp
strip.Color(red, green, blue)
```

The lines are the rows of a conventional 16 x 16 image, from top to bottom.
The matrix mapping function in the firmware handles the physical bottom-up,
serpentine wiring.

The files contain only the initializer rows, not the declaration around them.
Copy a complete file between the braces here:

```cpp
uint32_t colorMatrix[16][16] = {
  // Paste the .txt file here.
};
```

The matching sketch is:
`firmware/static_image/NeoPixel_Array/NeoPixel_Array.ino`.

## Included pictures

| File | Original local name | Subject |
| --- | --- | --- |
| `frog.txt` | `beka.txt` | Frog |
| `mount_fuji.txt` | `Fuji1.txt` | Mount Fuji and pagoda |
| `mario.txt` | `mario.txt` | Mario |
| `fox.txt` | `roka.txt` | Fox |
| `sakura.txt` | `sakura1.txt` | Sakura scene |
| `smurf_with_spear.txt` | `torp_dardaval.txt` | Smurf with a spear |

The current static firmware contains the same pixel values as
`mount_fuji.txt`.

## Creating or updating a picture

Run the Tkinter editor from the repository root:

```bash
python tools/pixel_editor/NeoPixel_Matrix_Canvas.py
```

Choose **Export to TXT**. The editor writes `export.txt` to the current working
directory. Check that it contains 16 rows with 16 colors per row, rename it to
a descriptive lowercase filename, and place it in `assets/patterns/txt`.

The generic `export.txt` and `export.mtx` output names are ignored by Git so an
accidental export is not committed as a new asset.
