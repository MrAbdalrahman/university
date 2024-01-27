import importlib
import subprocess
import xml.etree.ElementTree as ET
import re
from xml.dom import minidom
import os
from collections import Counter
import threading

# UI/UX
from pathlib import Path
from tkinter import Tk, Canvas, Entry, Text, Button, PhotoImage
import tkinter as tk
from tkinter import font


commands = None
is_thread_running = False


class CommandManual:
    """
    class to represent an individual manual for a Python command
    It handles data gathering and formatting into XML files
    """

    def __init__(self, command):
        self.command = command
        self.manual_data = {
            "description": "",
            "version": "",
            "example": "",
            "related_commands": "",
            "syntax": "",
            "documentation_link": ""
        }

    def fetch_command_description(self):
        """
        fetches the description of the command using subprocess.Popen
        """
        try:
            result = subprocess.Popen(["python", "-m", "pydoc", self.command], stdout=subprocess.PIPE)
            output, _ = result.communicate()
            self.manual_data["description"] = output.decode()
        except Exception as e:
            self.manual_data["description"] = f"Error fetching description: {e}"

    import importlib.metadata

    def fetch_version_history(self):
        """
        try to fetch the version of the package the command belongs to
        """
        try:
            # this works if the command is the name of a top-level package.
            version = importlib.metadata.version(self.command)
            self.manual_data["version"] = version
        except importlib.metadata.PackageNotFoundError:
            # if the package/command isn't found, it might be a built-in or not a top-level package.

            """
            fetches the version history of the command using subprocess.run
            """
            try:
                result = subprocess.run(["python", "--version"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                self.manual_data["version"] = result.stdout.decode().strip()
            except Exception as e:
                self.manual_data["version"] = f"Error fetching version: {e}"


    def fetch_examples_from_file(self, examples_file):
        """
        fetches only the example portion of the command usage from a local file
        the file should contain mappings of commands to their examples in the format "command: example - explanation"
        """
        try:
            with open(examples_file, "r") as file:
                examples = file.readlines()

            # Finding the example for the specific command and extracting only the "example" part
            example_line = next((line for line in examples if line.startswith(self.command + ":")), "")
            example = example_line.split(" - ")[0].split(":")[1].strip() if example_line else "No example available."
            self.manual_data["example"] = example

        except Exception as e:
            self.manual_data["example"] = f"Error fetching example: {e}"

    def fetch_related_commands(self, known_commands_file):
        """
        fetches related commands by searching for known Python built-ins and standard library modules in the documentation.
        Reads the known commands from a file 'known_commands.txt'
        """
        try:
            # Reading known commands from the file
            with open(known_commands_file, "r") as file:
                known_commands = [line.strip() for line in file if line.strip()]

            result = subprocess.Popen(["python", "-m", "pydoc", self.command], stdout=subprocess.PIPE)
            output, _ = result.communicate()
            doc = output.decode()

            # Find occurrences of these known commands in the documentation
            found_commands = set(command for command in known_commands if command in doc)

            # Removing the command itself from the list
            found_commands.discard(self.command)

            self.manual_data["related_commands"] = ', '.join(found_commands)

        except Exception as e:
            self.manual_data["related_commands"] = f"Error fetching related commands: {e}"

    def set_syntax_and_usage(self):
        """
        Sets the syntax and usage patterns of the command by fetching it from the command's documentation.
        Looks for lines that start with the command name, 'class {command}', or ' |  command'
        """
        try:
            # fetching the documentation of the command
            result = subprocess.Popen(["python", "-m", "pydoc", self.command], stdout=subprocess.PIPE)
            output, _ = result.communicate()
            doc = output.decode()

            # regular expression to extract lines matching the specific patterns
            pattern = re.compile(
                r'^\s*(class\s+' + re.escape(self.command) + r'\b.*|' +
                r'\|\s*' + re.escape(self.command) + r'\b.*|' +
                re.escape(self.command) + r'\b.*)', re.MULTILINE)
            matches = pattern.findall(doc)

            if matches:
                # joining all matching lines for syntax information
                self.manual_data["syntax"] = ' '.join(matches).strip()
            else:
                self.manual_data["syntax"] = "Syntax information not found."

        except Exception as e:
            self.manual_data["syntax"] = f"Error fetching syntax: {e}"



    def set_documentation_link(self, link):
        """
        Sets the online documentation link for the command based on predefined tuples.
        """

        # List of tuples with (command, classifacation number)
        classifacations = [
            ("help", 1), ("print", 1), ("list",2 ),
            ("dict",2 ), ("int", 1), ("str",2 ), ("max",1 ),
            ("min",1 ), ("map",1 ), ("len",1 ), ("sum",1 ),
            ("sorted",1 ), ("range",2 ), ("enumerate", 1), ("zip",1 ),
            ("filter", 1), ("type",1 ), ("abs",1 ), ("round",1 ),
            ("all",1 ),
        ]

        classifacationsURLS = {
            1: "https://docs.python.org/3/library/functions.html#",
            2: "https://docs.python.org/3/library/stdtypes.html#"
        }

        classifacationNum = next((case for cmd, case in classifacations if cmd == self.command), None)

        # making the full URL
        if classifacationNum:
            self.manual_data["documentation_link"] = classifacationsURLS[classifacationNum] + self.command
        else:
            self.manual_data["documentation_link"] = f"No documentation available for {self.command}"

    def generate_xml_manual(self):
        """
        generates an XML file for the command manual with new lines between each element
        """
        try:

            os.makedirs('manuals', exist_ok=True)

            file_path = os.path.join('manuals', f"{self.command}_manual.xml")
            # creating the root element with a generic name
            root = ET.Element("CommandManual")

            # adding sub-elements for each piece of manual data
            for key, value in self.manual_data.items():
                child = ET.SubElement(root, key)
                child.text = value

            # using minidom to format the output with new lines and indentation
            rough_string = ET.tostring(root, 'utf-8')
            reparsed = minidom.parseString(rough_string)
            pretty_xml_as_string = reparsed.toprettyxml(indent="  ")

            # writing the formatted XML to a file

            with open(file_path, "w") as file:
                file.write(pretty_xml_as_string)


        except Exception as e:
            print(f"Error generating XML manual: {e}")


class CommandManualGenerator:
    """
    class to orchestrate the generation of manuals for Python commands
    It reads commands from an input file and generates manuals
    """

    def __init__(self, input_file):
        self.input_file = input_file
        self.commands = []

    def read_commands(self):
        """
        reads commands from the input file
        """
        with open(self.input_file, 'r') as file:
            self.commands = [line.strip() for line in file.readlines()]

    def generate_all_manuals(self):
        def generate():
            global commands
            entry_4.delete('1.0', tk.END)
            if commands is not None:
                try:
                    compile_manuals_to_xml()
                    entry_4.insert("1.0", "all_manuals file generated\n")
                    for command in commands:
                        manual = CommandManual(command)
                        manual.fetch_command_description()
                        manual.fetch_version_history()
                        manual.fetch_examples_from_file("examples.txt")
                        manual.fetch_related_commands("known_commands.txt")
                        manual.set_syntax_and_usage()
                        manual.set_documentation_link("https://docs.python.org/3/")
                        manual.generate_xml_manual()
                        entry_4.insert(tk.END, f"Manual generated for command: {command}\n")
                    entry_4.insert(tk.END, "All manuals generated successfully.\n")
                except FileNotFoundError as e:
                    entry_4.insert(tk.END, f"Error: {e}\n")
                finally:
                    # use after method to schedule enable_buttons on the main thread
                    window.after(0, enable_buttons)
            else:
                entry_4.insert(tk.END, "No commands in the system. Please read commands first.\n")
                window.after(0, enable_buttons)

        # start the generation process in a new thread and disable buttons
        disable_buttons()
        thread = threading.Thread(target=generate)
        thread.start()


def compile_manuals_to_xml():
    """
    compiles all XML manuals in the current directory into a single XML file 'all_manuals.xml'
    """
    # creating the root element for the combined XML file
    root = ET.Element("AllCommandManuals")

    for filename in os.listdir('manuals'):
        if filename.endswith('_manual.xml'):
            # parsing each XML manual and adding its content to the root
            tree = ET.parse(os.path.join('manuals', filename))
            manual_root = tree.getroot()

            # creating a new element for each command manual
            command_element = ET.SubElement(root, "CommandManual", {'name': filename.replace('_manual.xml', '')})

            # copying all child elements from the individual manual to the new element
            for child in manual_root:
                ET.SubElement(command_element, child.tag).text = child.text

    rough_string = ET.tostring(root, 'utf-8')
    reparsed = minidom.parseString(rough_string)
    pretty_xml_as_string = reparsed.toprettyxml(indent="  ")

    # writing the combined XML to a file
    with open("all_manuals.xml", "w") as file:
        file.write(pretty_xml_as_string)


def normalize_xml_element(element):
    """
    normalize an XML element's text and sub-elements for comparison
    """
    # sort sub-elements by tag name
    sorted_children = sorted(element, key=lambda x: x.tag)

    # normalize text (strip whitespace and line breaks)
    text = (element.text or "").strip().replace('\n', ' ').replace('\r', ' ')

    return element.tag, text, [(child.tag, normalize_xml_element(child)) for child in sorted_children]


def normalize_text(text):
    # normalize text for comparison
    return re.findall(r'\b\w+\b', text.lower())

def compare_texts(text1, text2):
    # compare two pieces of text and return differences
    words1 = Counter(normalize_text(text1))
    words2 = Counter(normalize_text(text2))

    diff1 = words1 - words2
    diff2 = words2 - words1

    return ' '.join(diff1), ' '.join(diff2)

def disable_buttons():
    button_1['state'] = 'disabled'
    button_2['state'] = 'disabled'
    button_3['state'] = 'disabled'
    button_4['state'] = 'disabled'
    button_5['state'] = 'disabled'
    button_6['state'] = 'disabled'

def enable_buttons():
    button_1['state'] = 'normal'
    button_2['state'] = 'normal'
    button_3['state'] = 'normal'
    button_4['state'] = 'normal'
    button_5['state'] = 'normal'
    button_6['state'] = 'normal'


def extract_related_commands(xml_file):

    tree = ET.parse(xml_file)
    root = tree.getroot()
    related_commands_data = {}

    for command_manual in root.findall('CommandManual'):
        command_name = command_manual.get('name')

        # find the related_commands element
        related_commands_element = command_manual.find('related_commands')

        # check if the element exists and extract text
        if related_commands_element is not None and related_commands_element.text:
            related_commands = related_commands_element.text.split(', ')
        else:
            related_commands = []

        related_commands_data[command_name] = related_commands

    return related_commands_data


def get_command_recommendations(command_name, related_commands_data):
    return related_commands_data.get(command_name, [])

def print_command_file(command):
    filename = f"manuals/{command}_manual.xml"
    if os.path.exists(filename):
        entry_4.insert("1.0", f"{command}_manual.xml content:\n")
        with open(filename, 'r') as file:
            entry_4.insert(tk.END, file.read())
        entry_4.insert(tk.END, "\n--------------------\n")
    else:
        entry_4.insert("1.0", f"No specific manual found for command: {command}\n")

def print_lines_containing_command(command, all_manuals_file):
    all_manuals_path = f"{all_manuals_file}"
    if os.path.exists(all_manuals_path):
        with open(all_manuals_path, 'r') as file:
            for line in file:
                if command in line:
                    entry_4.insert(tk.END, line.strip() + "\n")
    else:
        entry_4.insert("1.0", f"File {all_manuals_file} not found.\n")

def perform_search(command):
    entry_4.delete('1.0', tk.END)
    entry_4.insert("1.0", f"Searching for command: {command}\n")

    # print specific command file content
    entry_4.insert(tk.END, "Command Manual Content:\n")
    print_command_file(command)

    # print relevant lines from all_manuals.xml
    entry_4.insert(tk.END, "\nOccurrences in all manuals:\n")
    print_lines_containing_command(command, "all_manuals.xml")

def read_commands():
    global commands
    entry_4.delete('1.0', tk.END)
    try:
        with open('commands.txt', 'r') as file:
            commands = file.read().splitlines()
        entry_4.insert("1.0","all commands read successfully")
    except FileNotFoundError as e:
        entry_4.insert("1.0",f"error: {e}")




def view_manual():
    global commands
    entry_4.delete('1.0', tk.END)
    if commands is not None:
        command = entry_1.get()  # Read text from entry_1
        if command:
            # Adjust the path here to point to the 'manuals' folder
            filename = f"manuals/{command}_manual.xml"  # Assuming 'manuals' folder is in the current directory
            if os.path.exists(filename):
                with open(filename, 'r') as file:
                    manual_content = file.read()

                # Clear existing content and insert new content into entry_4
                entry_4.delete('1.0', tk.END)
                entry_4.insert('1.0', manual_content)
            else:
                entry_4.insert("1.0", f"Manual for {command} not found.")
        else:
            entry_4.insert("1.0", "Please select a command.")
    else:
        entry_4.insert("1.0", "No commands in the system. Please read commands first.")


def verify_command():
    entry_4.delete('1.0', tk.END)
    if commands is not None:
        # open the files and read them in chunks
        try:
            with open("old.xml", "r") as file1, open("all_manuals.xml", "r") as file2:
                lines1 = file1.readlines()
                lines2 = file2.readlines()
        except Exception as e:
            entry_4.insert('1.0', f"Error reading files: {e}")
            return

        differences_old = []
        differences_all_manuals = []

        # define chunk size
        chunk_size = 100

        # process files in chunks
        for i in range(0, len(lines1), chunk_size):
            chunk1 = lines1[i:i + chunk_size]
            chunk2 = lines2[i:i + chunk_size]

            for line1, line2 in zip(chunk1, chunk2):
                diff_old, diff_all_manuals = compare_texts(line1, line2)

                if diff_old:
                    differences_old.append(f"'{diff_old}' exists in old.xml but not in all_manuals.xml")
                if diff_all_manuals:
                    differences_all_manuals.append(f"'{diff_all_manuals}' exists in all_manuals.xml but not in old.xml")

            # ipdate the UI with the progress
            entry_4.insert('1.0', f"Processed {i + chunk_size} lines...\n")

        # final update
        if differences_old or differences_all_manuals:
            for diff in differences_old:
                entry_4.insert('1.0', diff + '\n')
            for diff in differences_all_manuals:
                entry_4.insert('1.0', diff + '\n')
        else:
            entry_4.insert('1.0', "No differences found.\n")

    else:
        entry_4.insert("1.0", "No commands in the system. Please read commands first.\n")


def search_commands():

    global commands
    entry_4.delete('1.0', tk.END)
    if commands is not None:
        lookFor = entry_3.get()
        if lookFor:
            perform_search(lookFor)
        else:
            entry_4.insert('1.0', "Please enter a search query.")
    else:
        entry_4.insert("1.0", f"no commands in the system. please read commands at first")


def fetch_and_show_recommendations():

    global commands
    entry_4.delete('1.0', tk.END)
    if commands is not None:

        related_commands_data = extract_related_commands("all_manuals.xml")
        command = entry_2.get()
        if command:
            recommendations = get_command_recommendations(command, related_commands_data)
            if recommendations:
                entry_4.insert('1.0', "\n".join(recommendations))
            else:
                entry_4.insert('1.0', "No recommendations available.")
        else:
            entry_4.insert("1.0", "Please enter a command.")

    else:
        entry_4.insert("1.0", f"no commands in the system. please read commands at first")



#-----------------------------------UI/UX-----------------------------------------

OUTPUT_PATH = Path(__file__).parent
ASSETS_PATH = OUTPUT_PATH / Path(r"assets\frame0")



def relative_to_assets(path: str) -> Path:
    return ASSETS_PATH / Path(path)


window = Tk()
custom_font = font.Font(size=18)

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
    command=lambda:  CommandManualGenerator.generate_all_manuals(None),
    relief="flat"
)
button_1.place(
    x=2.0,
    y=91.0,
    width=266.0,
    height=54.0
)

button_image_6 = PhotoImage(
    file=relative_to_assets("button_6.png"))
button_6 = Button(
    image=button_image_6,
    borderwidth=0,
    highlightthickness=0,
    command=lambda:  read_commands(),
    relief="flat"
)
button_6.place(
    x=275.0,
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
    command=lambda:verify_command() ,
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
    command=lambda: view_manual(),
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
    command=lambda: search_commands(),
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
    command=lambda: fetch_and_show_recommendations(),
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
    highlightthickness=0,
    font=custom_font
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
    highlightthickness=0,
    font = custom_font
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
    highlightthickness=0,
font = custom_font
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
    highlightthickness=0,
    font = custom_font
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
