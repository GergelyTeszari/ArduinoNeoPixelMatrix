import tkinter as tk
from tkinter import colorchooser, messagebox

class NeoPixelCanvas:
    def __init__(self, master):
        self.master = master
        self.master.title("NeoPixel Matrix Drawer")
        
        self.canvas_size = 16
        self.pixel_size = 30
        self.colors = [[(0, 0, 0) for _ in range(self.canvas_size)] for _ in range(self.canvas_size)]  # Default color is black
        self.is_drawing = False  # Flag for drawing state
        self.dimmer_value = 100  # Dimmer percentage (100% means no dimming)

        self.sidebar = tk.Frame(master)
        self.sidebar.pack(side=tk.RIGHT)

        self.canvas = tk.Canvas(master, width=self.canvas_size * self.pixel_size, height=self.canvas_size * self.pixel_size)
        self.canvas.pack(side=tk.LEFT)

        self.color_button = tk.Button(self.sidebar, text="Select Color", command=self.choose_color)
        self.color_button.pack()

        self.eraser_button = tk.Button(self.sidebar, text="Eraser", command=self.use_eraser)
        self.eraser_button.pack()

        self.clear_button = tk.Button(self.sidebar, text="Clear Canvas", command=self.clear_canvas)
        self.clear_button.pack()

        self.export_txt_button = tk.Button(self.sidebar, text="Export to TXT", command=self.export_to_txt)
        self.export_txt_button.pack()

        self.export_mtx_button = tk.Button(self.sidebar, text="Export to MTX", command=self.export_to_mtx)
        self.export_mtx_button.pack()

        self.dimmer_label = tk.Label(self.sidebar, text="Dimmer (%)")
        self.dimmer_label.pack()
        self.dimmer_scale = tk.Scale(self.sidebar, from_=0, to=100, orient=tk.HORIZONTAL, command=self.update_dimmer)
        self.dimmer_scale.set(self.dimmer_value)
        self.dimmer_scale.pack()

        self.current_color = (255, 255, 255)  # Default drawing color (white)
        self.is_erasing = False  # State for eraser tool

        self.create_canvas()
        self.canvas.bind("<Button-1>", self.paint)
        self.canvas.bind("<B1-Motion>", self.paint)  # Continuous drawing with left mouse button
        self.canvas.bind("<ButtonRelease-1>", self.stop_drawing)
        self.master.bind("<Control-ButtonRelease-1>", self.stop_drawing)

    def create_canvas(self):
        for row in range(self.canvas_size):
            for col in range(self.canvas_size):
                x1 = col * self.pixel_size
                y1 = row * self.pixel_size
                x2 = x1 + self.pixel_size
                y2 = y1 + self.pixel_size
                self.canvas.create_rectangle(x1, y1, x2, y2, outline="white", fill="black")  # Pixels are black, borders are white

    def choose_color(self):
        color_code = colorchooser.askcolor(title="Choose color")
        if color_code[0]:
            self.current_color = tuple(map(int, color_code[0]))
            self.is_erasing = False  # Switch off eraser when a color is selected

    def use_eraser(self):
        self.is_erasing = True
        self.current_color = (0, 0, 0)  # Set color to black for erasing

    def paint(self, event):
        col = event.x // self.pixel_size
        row = event.y // self.pixel_size
        if 0 <= col < self.canvas_size and 0 <= row < self.canvas_size:
            self.colors[row][col] = self.current_color
            x1 = col * self.pixel_size
            y1 = row * self.pixel_size
            x2 = x1 + self.pixel_size
            y2 = y1 + self.pixel_size
            fill_color = self.rgb_to_hex(self.current_color)
            self.canvas.create_rectangle(x1, y1, x2, y2, outline="white", fill=fill_color)

    def stop_drawing(self, event):
        self.is_drawing = False  # Stop drawing when mouse button is released

    def clear_canvas(self):
        self.colors = [[(0, 0, 0) for _ in range(self.canvas_size)] for _ in range(self.canvas_size)]
        self.create_canvas()  # Recreate the canvas to reset it

    def export_to_txt(self):
        output = ""
        for row in range(self.canvas_size):
            row_colors = []
            for col in range(self.canvas_size):
                r, g, b = self.colors[row][col]
                dimmed_r = int(r * (self.dimmer_value / 100))
                dimmed_g = int(g * (self.dimmer_value / 100))
                dimmed_b = int(b * (self.dimmer_value / 100))
                row_colors.append(f"strip.Color({dimmed_r}, {dimmed_g}, {dimmed_b}), ")
            output += "{" + "".join(row_colors) + "},\n"
        
        with open("export.txt", "w") as f:
            f.write(output.strip())
        
        messagebox.showinfo("Export", "Colors exported to export.txt")

    def export_to_mtx(self):
        output = ""
        for row in range(self.canvas_size):
            row_colors = []
            for col in range(self.canvas_size):
                r, g, b = self.colors[row][col]
                dimmed_r = int(r * (self.dimmer_value / 100))
                dimmed_g = int(g * (self.dimmer_value / 100))
                dimmed_b = int(b * (self.dimmer_value / 100))
                row_colors.append(f"{dimmed_r},{dimmed_g},{dimmed_b}")
            output += ";".join(row_colors) + "\n"

        with open("export.mtx", "w") as f:
            f.write(output.strip())
        
        messagebox.showinfo("Export", "Colors exported to export.mtx")

    def update_dimmer(self, value):
        self.dimmer_value = int(value)

    def rgb_to_hex(self, rgb):
        return f"#{rgb[0]:02x}{rgb[1]:02x}{rgb[2]:02x}"

if __name__ == "__main__":
    root = tk.Tk()
    app = NeoPixelCanvas(root)
    root.mainloop()
