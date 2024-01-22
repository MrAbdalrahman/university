# Implementing the details of the CommandManual and CommandManualGenerator classes

import subprocess
import xml.etree.ElementTree as ET
import re
from xml.dom import minidom
import os

# UI/UX
from pathlib import Path
from tkinter import Tk, Canvas, Entry, Text, Button, PhotoImage
import tkinter as tk
from tkinter import font


commands = None
class CommandManual:
    """
    Class to represent an individual manual for a Python command.
    It handles data gathering and formatting into XML files.
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
        Fetches the description of the command using subprocess.Popen.
        """
        try:
            result = subprocess.Popen(["python", "-m", "pydoc", self.command], stdout=subprocess.PIPE)
            output, _ = result.communicate()
            self.manual_data["description"] = output.decode()
        except Exception as e:
            self.manual_data["description"] = f"Error fetching description: {e}"

    def fetch_version_history(self):
        """
        Fetches the version history of the command using subprocess.run.
        """
        try:
            result = subprocess.run(["python", "--version"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            self.manual_data["version"] = result.stdout.decode().strip()
        except Exception as e:
            self.manual_data["version"] = f"Error fetching version: {e}"

    def fetch_examples_from_file(self, examples_file):
        """
        Fetches only the example portion of the command usage from a local file.
        The file should contain mappings of commands to their examples in the format "command: example - explanation".
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
        Fetches related commands by searching for known Python built-ins and standard library modules in the documentation.
        Reads the known commands from a file 'known_commands.txt'.
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
        Looks for lines that start with the command name, 'class {command}', or ' |  command'.
        """
        try:
            # Fetching the documentation of the command
            result = subprocess.Popen(["python", "-m", "pydoc", self.command], stdout=subprocess.PIPE)
            output, _ = result.communicate()
            doc = output.decode()

            # Regular expression to extract lines matching the specific patterns
            pattern = re.compile(
                r'^\s*(class\s+' + re.escape(self.command) + r'\b.*|' +
                r'\|\s*' + re.escape(self.command) + r'\b.*|' +
                re.escape(self.command) + r'\b.*)', re.MULTILINE)
            matches = pattern.findall(doc)

            if matches:
                # Joining all matching lines for syntax information
                self.manual_data["syntax"] = ' '.join(matches).strip()
            else:
                self.manual_data["syntax"] = "Syntax information not found."

        except Exception as e:
            self.manual_data["syntax"] = f"Error fetching syntax: {e}"

    # Note: This implementation looks for multiple patterns that are likely to contain the syntax information.


    def set_documentation_link(self, link):
        """
        Sets the online documentation link for the command.
        """
        self.manual_data["documentation_link"] = link

    def generate_xml_manual(self):
        """
        Generates an XML file for the command manual with new lines between each element.
        """
        try:
            # Creating the root element with a generic name (e.g., 'CommandManual')
            root = ET.Element("CommandManual")

            # Adding sub-elements for each piece of manual data
            for key, value in self.manual_data.items():
                child = ET.SubElement(root, key)
                child.text = value

            # Using minidom to format the output with new lines and indentation
            rough_string = ET.tostring(root, 'utf-8')
            reparsed = minidom.parseString(rough_string)
            pretty_xml_as_string = reparsed.toprettyxml(indent="  ")

            # Writing the formatted XML to a file
            with open(f"{self.command}_manual.xml", "w") as file:
                file.write(pretty_xml_as_string)

        except Exception as e:
            print(f"Error generating XML manual: {e}")

    # Note: This implementation uses minidom for pretty-printing the XML, which adds new lines and indentation.


class CommandManualGenerator:
    """
    Class to orchestrate the generation of manuals for Python commands.
    It reads commands from an input file and generates manuals.
    """

    def __init__(self, input_file):
        self.input_file = input_file
        self.commands = []

    def read_commands(self):
        """
        Reads commands from the input file.
        """
        with open(self.input_file, 'r') as file:
            self.commands = [line.strip() for line in file.readlines()]

    def generate_manuals(self,examples_file,known_commands_file):
        """
        Generates manuals for each command read from the input file.
        """
        for command in self.commands:
            manual = CommandManual(command)
            manual.fetch_command_description()
            manual.fetch_version_history()
            manual.fetch_examples_from_file(examples_file)
            manual.fetch_related_commands(known_commands_file)
            manual.set_syntax_and_usage()
            manual.set_documentation_link("https://docs.python.org/3/")
            manual.generate_xml_manual()


def compile_manuals_to_xml():
    """
    Compiles all XML manuals in the current directory into a single XML file 'all_manuals.xml'.
    """
    # Creating the root element for the combined XML file
    root = ET.Element("AllCommandManuals")

    for filename in os.listdir('.'):
        if filename.endswith('_manual.xml'):
            # Parsing each XML manual and adding its content to the root
            tree = ET.parse(filename)
            manual_root = tree.getroot()

            # Creating a new element for each command manual
            command_element = ET.SubElement(root, "CommandManual", {'name': filename.replace('_manual.xml', '')})

            # Copying all child elements from the individual manual to the new element
            for child in manual_root:
                ET.SubElement(command_element, child.tag).text = child.text

    # Pretty-printing the combined XML content
    rough_string = ET.tostring(root, 'utf-8')
    reparsed = minidom.parseString(rough_string)
    pretty_xml_as_string = reparsed.toprettyxml(indent="  ")

    # Writing the combined XML to a file
    with open("all_manuals.xml", "w") as file:
        file.write(pretty_xml_as_string)


def normalize_xml_element(element):
    """Normalize an XML element's text and sub-elements for comparison."""
    # Sort sub-elements by tag name
    sorted_children = sorted(element, key=lambda x: x.tag)

    # Normalize text (strip whitespace and line breaks)
    text = (element.text or "").strip().replace('\n', ' ').replace('\r', ' ')

    return element.tag, text, [(child.tag, normalize_xml_element(child)) for child in sorted_children]


def compare_xml_files():
    """
    Compares the content of 'old.xml' and 'all_manuals.xml' and reports differences.
    Focuses on normalized textual content to account for minor formatting differences.
    """
    # Parsing the XML files
    tree1 = ET.parse("old.xml")
    root1 = tree1.getroot()

    tree2 = ET.parse("all_manuals.xml")
    root2 = tree2.getroot()

    differences = {}

    # Create normalized representations of each XML file
    content_root1 = {child.tag: normalize_xml_element(child) for child in root1}
    content_root2 = {child.tag: normalize_xml_element(child) for child in root2}

    # Compare the normalized content
    for tag, content in content_root1.items():
        if content != content_root2.get(tag):
            differences[tag] = f"Content mismatch in tag: {tag}"

    return differences


def extract_related_commands(xml_file):

    tree = ET.parse(xml_file)
    root = tree.getroot()
    related_commands_data = {}

    for command_manual in root.findall('CommandManual'):
        command_name = command_manual.get('name')

        # Find the related_commands element
        related_commands_element = command_manual.find('related_commands')

        # Check if the element exists and extract text
        if related_commands_element is not None and related_commands_element.text:
            related_commands = related_commands_element.text.split(', ')
        else:
            related_commands = []

        related_commands_data[command_name] = related_commands

    return related_commands_data


def get_command_recommendations(command_name, related_commands_data):
    return related_commands_data.get(command_name, [])

def print_command_file(command):
    filename = f"{command}_manual.xml"
    if os.path.exists(filename):
        entry_4.insert("1.0",f"{command}_manual.xml content:")
        with open(filename, 'r') as file:
            entry_4.insert("1.0", file.read())
        entry_4.insert("1.0", "--------------------")
    else:
        entry_4.insert("1.0", f"No specific manual found for command: {command}")

def print_lines_containing_command(command, all_manuals_file):

    with open(all_manuals_file, 'r') as file:
        for line in file:
            if command in line:
                entry_4.insert("1.0", line.strip())

def perform_search(command):


    entry_4.delete('1.0', tk.END)
    entry_4.insert("1.0", f"Searching for command: {command}\n")


    # Part 1: Print specific command file

    entry_4.insert("1.0", "Command Manual Content:")

    print_command_file(command)

    # Part 2: Print relevant lines from all_manuals.xml
    entry_4.insert("1.0", "\nOccurrences in all manuals:")

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

def generate_all_manuals():
    global commands
    entry_4.delete('1.0', tk.END)
    if commands != None:
        try:
            compile_manuals_to_xml()
            entry_4.insert("1.0", f"all_manuals file generated ")
            for command in commands:
                manual = CommandManual(command)
                manual.fetch_command_description()
                manual.fetch_version_history()
                manual.fetch_examples_from_file("examples.txt")  # Adjust file name/path as necessary
                manual.fetch_related_commands("known_commands.txt")  # Adjust file name/path as necessary
                manual.set_syntax_and_usage()
                manual.set_documentation_link("https://docs.python.org/3/")
                manual.generate_xml_manual()
                entry_4.insert("1.0", f"Manual generated for command: {command}\n")
            entry_4.insert("1.0", "All manuals generated successfully.")

        except FileNotFoundError as e:
            entry_4.insert("1.0", f"Error: {e}")
    else:
        entry_4.insert("1.0", f"no commands in the system. please read commands at first")
def view_manual():
    global commands
    entry_4.delete('1.0', tk.END)
    if commands is not None:

        command = entry_1.get()  # Read text from entry_1
        if command:
            filename = f"{command}_manual.xml"
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
        entry_4.insert("1.0", f"no commands in the system. please read commands at first")


def verify_command():
    entry_4.delete('1.0', tk.END)
    if commands != None:

        differences = compare_xml_files()

        if differences:
            entry_4.insert('1.0', "Files verified. Differences detected.\n")
        else:
            entry_4.insert('1.0', "Files verified. No differences detected.")

    else:
        entry_4.insert("1.0", f"no commands in the system. please read commands at first")



def search_commands():

    global commands
    entry_4.delete('1.0', tk.END)
    if commands is not None:
        query = entry_3.get()
        if query:
            perform_search(query)
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
custom_font = font.Font(family="Helvetica", size=18)  # Example: Helvetica font, size 12

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
    command=lambda: generate_all_manuals(),
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
