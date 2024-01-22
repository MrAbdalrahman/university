

from pathlib import Path
from tkinter import Tk, Canvas, Entry, Text, Button, PhotoImage


OUTPUT_PATH = Path(__file__).parent
ASSETS_PATH = OUTPUT_PATH / Path(r"C:\Users\mrabd\Desktop\build\assets\frame0")


def relative_to_assets(path: str) -> Path:
    return ASSETS_PATH / Path(path)


window = Tk()

window.geometry("605x771")
window.configure(bg = "#FAEAF0")


canvas = Canvas(
    window,
    bg = "#FAEAF0",
    height = 771,
    width = 605,
    bd = 0,
    highlightthickness = 0,
    relief = "ridge"
)

canvas.place(x = 0, y = 0)
button_image_1 = PhotoImage(
    file=relative_to_assets("button_1.png"))
button_1 = Button(
    image=button_image_1,
    borderwidth=0,
    highlightthickness=0,
    command=lambda: print("button_1 clicked"),
    relief="flat"
)
button_1.place(
    x=2.0,
    y=91.0,
    width=266.0,
    height=54.0
)

button_image_2 = PhotoImage(
    file=relative_to_assets("button_2.png"))
button_2 = Button(
    image=button_image_2,
    borderwidth=0,
    highlightthickness=0,
    command=lambda: print("button_2 clicked"),
    relief="flat"
)
button_2.place(
    x=2.0,
    y=150.0,
    width=266.0,
    height=54.0
)

button_image_3 = PhotoImage(
    file=relative_to_assets("button_3.png"))
button_3 = Button(
    image=button_image_3,
    borderwidth=0,
    highlightthickness=0,
    command=lambda: print("button_3 clicked"),
    relief="flat"
)
button_3.place(
    x=0.0,
    y=209.0,
    width=177.0,
    height=54.0
)

button_image_4 = PhotoImage(
    file=relative_to_assets("button_4.png"))
button_4 = Button(
    image=button_image_4,
    borderwidth=0,
    highlightthickness=0,
    command=lambda: print("button_4 clicked"),
    relief="flat"
)
button_4.place(
    x=0.0,
    y=275.0,
    width=177.0,
    height=54.0
)

button_image_5 = PhotoImage(
    file=relative_to_assets("button_5.png"))
button_5 = Button(
    image=button_image_5,
    borderwidth=0,
    highlightthickness=0,
    command=lambda: print("button_5 clicked"),
    relief="flat"
)
button_5.place(
    x=0.0,
    y=342.0,
    width=177.0,
    height=54.0
)

entry_image_1 = PhotoImage(
    file=relative_to_assets("entry_1.png"))
entry_bg_1 = canvas.create_image(
    396.5,
    244.5,
    image=entry_image_1
)
entry_1 = Entry(
    bd=0,
    bg="#C0B8EB",
    fg="#000716",
    highlightthickness=0
)
entry_1.place(
    x=214.0,
    y=217.0,
    width=365.0,
    height=53.0
)

entry_image_2 = PhotoImage(
    file=relative_to_assets("entry_2.png"))
entry_bg_2 = canvas.create_image(
    396.5,
    366.5,
    image=entry_image_2
)
entry_2 = Entry(
    bd=0,
    bg="#C0B8EB",
    fg="#000716",
    highlightthickness=0
)
entry_2.place(
    x=214.0,
    y=339.0,
    width=365.0,
    height=53.0
)

entry_image_3 = PhotoImage(
    file=relative_to_assets("entry_3.png"))
entry_bg_3 = canvas.create_image(
    396.5,
    305.5,
    image=entry_image_3
)
entry_3 = Entry(
    bd=0,
    bg="#C0B8EB",
    fg="#000716",
    highlightthickness=0
)
entry_3.place(
    x=214.0,
    y=278.0,
    width=365.0,
    height=53.0
)

entry_image_4 = PhotoImage(
    file=relative_to_assets("entry_4.png"))
entry_bg_4 = canvas.create_image(
    299.0,
    607.0,
    image=entry_image_4
)
entry_4 = Text(
    bd=0,
    bg="#CDBABF",
    fg="#000716",
    highlightthickness=0
)
entry_4.place(
    x=18.0,
    y=464.0,
    width=562.0,
    height=284.0
)

canvas.create_text(
    96.0,
    412.0,
    anchor="nw",
    text="Output",
    fill="#000000",
    font=("Inter Black", 24 * -1)
)

image_image_1 = PhotoImage(
    file=relative_to_assets("image_1.png"))
image_1 = canvas.create_image(
    302.0,
    43.0,
    image=image_image_1
)

image_image_2 = PhotoImage(
    file=relative_to_assets("image_2.png"))
image_2 = canvas.create_image(
    502.0,
    43.0,
    image=image_image_2
)
window.resizable(False, False)
window.mainloop()
