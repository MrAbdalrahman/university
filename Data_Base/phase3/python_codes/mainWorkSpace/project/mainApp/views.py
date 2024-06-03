from django.conf import settings
from django.shortcuts import render,redirect
from django.db import connection


from .forms import SignupForm,LoginForm
from django.contrib.sessions.models import Session
from django.contrib.auth import logout
from django.db import connections ,IntegrityError
from django.contrib import messages  
from django.views.decorators.http import require_POST
from django.views.decorators.csrf import csrf_exempt
from django.http import JsonResponse
import json
from django.views.decorators.http import require_http_methods
import openai
import os
from django.core.management import call_command
import copy




def index(request):
    request.session['is_logged_in'] = False  # Set a session flag
    return render(request, 'mainApp/index.html')

def aboutus(request):
    return render(request, 'mainApp/aboutus.html')

def contact(request):
    return render(request, 'mainApp/contact.html')


def signup(request):
    request.session['is_logged_in'] = False
    if request.method == 'POST':
        form = SignupForm(request.POST)
        if form.is_valid():
            username = form.cleaned_data['username']
            password = form.cleaned_data['password']

            # Insert new user into the database
            with connection.cursor() as cursor:
                cursor.execute("INSERT INTO users (username, password) VALUES (%s, %s)", [username, password])

            # Create a new database name based on the username
            db_name = f"clinic_user_{username}"

            # Create the new database
            with connection.cursor() as cursor:
                 cursor.execute(f"CREATE DATABASE {db_name}")

            # Create a deep copy of the current DATABASES settings
            new_databases = copy.deepcopy(settings.DATABASES)

            # Add the new database configuration
            new_db_config = {
                'ENGINE': 'django.db.backends.mysql',
                'NAME': db_name,
                'USER': 'root',
                'PASSWORD': '1234',
                'HOST': '',
                'PORT': '',
                'OPTIONS': settings.DATABASES['default'].get('OPTIONS', {}),
                'TIME_ZONE': settings.TIME_ZONE,
                'ATOMIC_REQUESTS': settings.DATABASES['default'].get('ATOMIC_REQUESTS', False),
                'AUTOCOMMIT': settings.DATABASES['default'].get('AUTOCOMMIT', True),
                'CONN_MAX_AGE': settings.DATABASES['default'].get('CONN_MAX_AGE', 0),
                'DISABLE_SERVER_SIDE_CURSORS': settings.DATABASES['default'].get('DISABLE_SERVER_SIDE_CURSORS', False),
                'CONN_HEALTH_CHECKS': settings.DATABASES['default'].get('CONN_HEALTH_CHECKS', False),
            }
            new_databases[db_name] = new_db_config

            # Temporarily add the new database configuration to the settings
            settings.DATABASES.update(new_databases)

            # Execute the SQL commands to create the tables in the new database
            create_tables_sql = """
            CREATE TABLE Patients (
                Patient_ID INT PRIMARY KEY,
                Name VARCHAR(255),
                DOB DATE,
                Address VARCHAR(255),
                Email VARCHAR(255)
            );
            CREATE TABLE Appointments (
                Appointment_ID INT PRIMARY KEY,
                Patient_ID INT,
                Date DATE,
                Time TIME,
                Purpose VARCHAR(255),
                FOREIGN KEY (Patient_ID) REFERENCES Patients(Patient_ID) ON DELETE CASCADE
            );
            CREATE TABLE Visits (
                patient_ID INT,
                Visit_ID INT PRIMARY KEY,
                Date DATE,
                Time TIME,
                FOREIGN KEY (Patient_ID) REFERENCES Patients(Patient_ID) ON DELETE CASCADE
            );
            CREATE TABLE Messages (
                Message_ID INT PRIMARY KEY,
                Appointment_ID INT,
                Patient_ID INT,
                visit_ID INT,
                Date DATE,
                Type VARCHAR(100),
                Content TEXT,
                FOREIGN KEY (Appointment_ID) REFERENCES Appointments(Appointment_ID) ON DELETE CASCADE,
                FOREIGN KEY (Patient_ID) REFERENCES Patients(Patient_ID) ON DELETE CASCADE,
                FOREIGN KEY (visit_ID) REFERENCES visits(visit_ID) ON DELETE CASCADE
            );
            CREATE TABLE Services (
                Service_ID INT PRIMARY KEY,
                Name VARCHAR(255),
                Description TEXT
            );
            CREATE TABLE Treatment_Records (
                Visit_ID INT,
                Record_ID INT PRIMARY KEY,
                Description TEXT,
                FOREIGN KEY (Visit_ID) REFERENCES Visits(Visit_ID) ON DELETE CASCADE
            );
            CREATE TABLE Contact_Numbers (
                Contact_ID INT AUTO_INCREMENT PRIMARY KEY,
                Patient_ID INT,
                Contact_number VARCHAR(50),
                FOREIGN KEY (Patient_ID) REFERENCES Patients(Patient_ID) ON DELETE CASCADE
            );
            CREATE TABLE service_Patient (
                patient_ID INT,
                service_ID INT,
                PRIMARY KEY (patient_ID, service_ID),
                FOREIGN KEY (Patient_ID) REFERENCES Patients(Patient_ID) ON DELETE CASCADE,
                FOREIGN KEY (service_ID) REFERENCES services(service_ID) ON DELETE CASCADE
            );
            CREATE TABLE service_visit (
                visit_ID INT,
                service_ID INT,
                PRIMARY KEY (visit_ID, service_ID),
                FOREIGN KEY (visit_ID) REFERENCES visits(visit_ID) ON DELETE CASCADE,
                FOREIGN KEY (service_ID) REFERENCES services(service_ID) ON DELETE CASCADE
            );
            CREATE TABLE service_message (
                message_ID INT,
                service_ID INT,
                PRIMARY KEY (message_ID, service_ID),
                FOREIGN KEY (message_ID) REFERENCES messages(message_ID) ON DELETE CASCADE,
                FOREIGN KEY (service_ID) REFERENCES services(service_ID) ON DELETE CASCADE
            );
            CREATE TABLE service_Treatment_record (
                Record_ID INT,
                service_ID INT,
                PRIMARY KEY (Record_ID, service_ID),
                FOREIGN KEY (Record_ID) REFERENCES Treatment_records(Record_ID) ON DELETE CASCADE,
                FOREIGN KEY (service_ID) REFERENCES services(service_ID) ON DELETE CASCADE
            );
            """

            # Execute the SQL commands to create the tables in the new database
            with connections[db_name].cursor() as cursor:
                for statement in create_tables_sql.split(";"):
                    if statement.strip():
                        cursor.execute(statement)

            # Save the new database configuration to a JSON file
            with open('db_configs.json', 'r') as f:
                db_configs = json.load(f)
            db_configs[db_name] = new_db_config
            with open('db_configs.json', 'w') as f:
                json.dump(db_configs, f, indent=4)

            return redirect('login')
        else:
            print("Form is not valid")
            print(form.errors)
    else:
        form = SignupForm()
    return render(request, 'mainApp/signup.html', {'form': form})

def user_login(request):
    request.session['is_logged_in'] = False  # Set a session flag
    if request.method == 'POST':
        form = LoginForm(request.POST)
        if form.is_valid():
            username = form.cleaned_data['username']
            password = form.cleaned_data['password']

            # Initialize user variable
            user = None

            # Retrieve only the needed user by username
            with connection.cursor() as cursor:
                cursor.execute("SELECT username, password FROM users WHERE username = %s", [username])
                result = cursor.fetchone()

                if result:
                    db_username, db_password = result
                    if username == db_username and password == db_password:
                        request.session['is_logged_in'] = True  # Set a session flag
                        request.session['username'] = db_username
                        request.session['db_name'] = 'clinic_user' + '_'+db_username   # Set the database name in the session

                        # Assuming you handle session creation or similar logic here
                        user = {'username': db_username}  # Simulate a user object
                        print("Login success")
                    else:
                        print("Login failed")

            # Redirect if user was successfully authenticated
            if user:
                return redirect('control')
            else:
                form.add_error(None, "Invalid username or password")
    else:
        print("Login failed")
        form = LoginForm()

    return render(request, 'mainApp/login.html', {'form': form})


def control_view(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in
    
    username = request.session.get('username')  # Make sure to use .get for safe access
    db = request.session.get('db_name')

    return render(request, 'mainApp/control.html', {'username': username})



def logout_view(request):
    logout(request)  # Django's logout function clears the session
    return redirect('index')  # Redirect to the index page after logging out

def patients_view(request):

    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in

    username = request.session.get('username')  # Make sure to use .get for safe access
    db = request.session.get('db_name')
    query = "SELECT patient_id, name, dob, address, email FROM patients"
    with connections[db].cursor() as cursor:  
        cursor.execute(query)
        rows = cursor.fetchall()

    patients = [
        {
            'patient_id': row[0],
            'name': row[1],
            'dob': row[2],
            'address': row[3],
            'email': row[4]
        } for row in rows
    ]
   

    return render(request, 'user/patients.html', {'patients': patients,
                                             'username': username})


def add_patient(request):

    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in
    
    if request.method == 'POST':
        patient_id = request.POST.get('patient_id')
        name = request.POST.get('name')
        dob = request.POST.get('dob')
        address = request.POST.get('address')
        email = request.POST.get('email')

        db = request.session.get('db_name', 'default')  # Use default if db_name is not in session

        query = "INSERT INTO patients (patient_id, name, dob, address, email) VALUES (%s, %s, %s, %s, %s)"
        params = [patient_id, name, dob, address, email]

        try:
            with connections[db].cursor() as cursor:
                cursor.execute(query, params)
            return redirect('patients')  # Redirect if no error
        except IntegrityError:
            messages.error(request, "A patient with the same ID already exists. Please use a different ID.")  # Add an error message

    # Always show the form if GET or if there was a POST error
    return render(request, 'user/addPatient.html')



@require_POST
@csrf_exempt
def delete_patients(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in

    db = request.session.get('db_name', 'default')  # Use default if db_name is not in session

    try:
        data = json.loads(request.body)
        patient_ids = data.get('patient_ids', [])

        # Use the 'db2' database connection
        with connections[db].cursor() as cursor:
            # Construct and execute the SQL query for each patient ID
            for patient_id in patient_ids:
                cursor.execute("DELETE FROM patients WHERE patient_id = %s", [patient_id])

        return JsonResponse({'success': True})
    except Exception as e:
        return JsonResponse({'success': False, 'error': str(e)})
    

@require_http_methods(["POST"])
def update_patient(request):
    if not request.session.get('is_logged_in', False):
            return redirect('login')  # Redirect to login if not logged in
    
    db = request.session.get('db_name', 'default')  # Use default if db_name is not in session


    data = json.loads(request.body)
    patient_id = data.get('patient_id')
    name = data.get('name')
    dob = data.get('dob')
    address = data.get('address')
    email = data.get('email')

    try:
        with connections[db].cursor() as cursor:
            cursor.execute("""
                UPDATE patients SET name = %s, dob = %s, address = %s, email = %s 
                WHERE patient_id = %s
            """, [name, dob, address, email, patient_id])
        return JsonResponse({'success': True})
    except Exception as e:
        return JsonResponse({'success': False, 'error': str(e)})
    

@require_http_methods(["POST"])
def get_patient_details(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in

    db = request.session.get('db_name', 'default')  # Use default if db_name is not in session

    data = json.loads(request.body)
    patient_id = data.get('patient_id')
    try:
        with connections[db].cursor() as cursor:
            cursor.execute("SELECT patient_id, name, dob, address, email FROM patients WHERE patient_id = %s", [patient_id])
            row = cursor.fetchone()
        if row:
            patient_data = {
                'patient_id': row[0],
                'name': row[1],
                'dob': row[2],
                'address': row[3],
                'email': row[4]
            }
            return JsonResponse({'success': True, 'patient': patient_data})
        else:
            return JsonResponse({'success': False, 'error': 'Patient not found'})
    except Exception as e:
        return JsonResponse({'success': False, 'error': str(e)})


@csrf_exempt
def process_query(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in

    db = request.session.get('db_name', 'default')  # Use default if db_name is not in session

    data = json.loads(request.body)
    nl_query = data['query']
    try:
        sql_query = translate_to_sql(nl_query)  # Function to translate NL to SQL
        results = execute_sql_query(sql_query, request)  # Function to execute SQL and return results
        print(results)
        return JsonResponse({'success': True, 'result': results})
    except Exception as e:
        return JsonResponse({'success': False, 'error': str(e)})



def translate_to_sql(nl_query):
    openai.api_key = settings.OPENAI_API_KEY

    if not openai.api_key:
        print("OpenAI API key is not set in environment variables")
    
    schema_context = """
    The database schema is as follows:
    Table: patients
    Columns:
    - patient_id (INTEGER)
    - name (TEXT)
    - dob (DATE)
    - address (TEXT)
    - email (TEXT)

    Table: contact_numbers
    Columns:
    - contact_id (INTEGER)
    - patient_id (INTEGER)
    - contact_number (TEXT)

    Table: messages
    Columns:
    - message_id (INTEGER)
    - appointment_id (INTEGER)
    - patient_id (INTEGER)
    - visit_id (INTEGER)
    - date (DATE)
    - type (TEXT)
    - content (TEXT)

    Table: appointments
    Columns:
    - appointment_id (INTEGER)
    - patient_id (INTEGER)
    - date (DATE)
    - time (TIME)
    - purpose (TEXT)

    Table: services
    Columns:
    - service_id (INTEGER)
    - name (TEXT)
    - description (TEXT)

    Table: treatment_records
    Columns:
    - visit_id (INTEGER)
    - record_id (INTEGER)
    - description (TEXT)

    Table: visits
    Columns:
    - patient_id (INTEGER)
    - visit_id (INTEGER)
    - date (DATE)
    - time (TIME)


    Note:
    - The 'dob' column stores dates in the format 'YYYY-MM-DD'.
    - Use standard SQL  functions compatible with most databases.
    - dont use any ready functions 
    - dont write ```sql ``` or ``` ``` around the query
    """

    try:
        response = openai.ChatCompletion.create(
            model="gpt-4",#"gpt-3.5-turbo",
            messages=[
                {"role": "system", "content": schema_context},
                {"role": "user", "content": f"Translate this natural language query into SQL: {nl_query}"}
            ],
            max_tokens=64
        )
        sql_query = response['choices'][0]['message']['content'].strip()
        print(sql_query)
        return sql_query
    except Exception as e:
        print(f"Error in generating SQL: {e}")
        return None


def execute_sql_query(sql_query, request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in

    db = request.session.get('db_name')  # Use default if db_name is not in session

    print(sql_query)

    with connections[db].cursor() as cursor:
        cursor.execute(sql_query)
        columns = [col[0] for col in cursor.description]
        return [dict(zip(columns, row)) for row in cursor.fetchall()]

def contact_numbers_view(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in

    username = request.session.get('username')  # Make sure to use .get for safe access
    db = request.session.get('db_name')
    query = "SELECT contact_id, patient_id, contact_number FROM contact_numbers"
    with connections[db].cursor() as cursor:  
        cursor.execute(query)
        rows = cursor.fetchall()

    contact_numbers = [
        {
            'contact_id': row[0],
            'patient_id': row[1],
            'contact_number': row[2]
        } for row in rows
    ]

   
    return render(request, 'user/contact_numbers.html', {'contact_numbers': contact_numbers,
                                                        'username': username})


def add_contact(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in
    
    if request.method == 'POST':
        contact_id = request.POST.get('contact_id')
        patient_id = request.POST.get('patient_id')
        contact_number = request.POST.get('contact_number')

        db = request.session.get('db_name', 'default')  # Use default if db_name is not in session

        query = "INSERT INTO contact_numbers (contact_id, patient_id, contact_number) VALUES (%s, %s, %s)"
        params = [contact_id, patient_id, contact_number]

        try:
            with connections[db].cursor() as cursor:
                cursor.execute(query, params)
            return redirect('contact_numbers')  # Redirect if no error
        except IntegrityError:
            messages.error(request, "A contact with the same ID already exists. Please use a different ID.")  # Add an error message

    # Always show the form if GET or if there was a POST error
    return render(request, 'user/addContact.html')


@require_POST
@csrf_exempt
def delete_contacts(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in

    db = request.session.get('db_name', 'default')  # Use default if db_name is not in session

    try:
        data = json.loads(request.body)
        contact_ids = data.get('contact_ids', [])

        # Use the 'db' database connection
        with connections[db].cursor() as cursor:
            # Construct and execute the SQL query for each contact ID
            for contact_id in contact_ids:
                cursor.execute("DELETE FROM contact_numbers WHERE contact_id = %s", [contact_id])

        return JsonResponse({'success': True})
    except Exception as e:
        return JsonResponse({'success': False, 'error': str(e)})
    

@require_http_methods(["POST"])
def update_contact(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in
    
    db = request.session.get('db_name', 'default')  # Use default if db_name is not in session

    data = json.loads(request.body)
    contact_id = data.get('contact_id')
    patient_id = data.get('patient_id')
    contact_number = data.get('contact_number')

    try:
        with connections[db].cursor() as cursor:
            cursor.execute("""
                UPDATE contact_numbers SET patient_id = %s, contact_number = %s 
                WHERE contact_id = %s
            """, [patient_id, contact_number, contact_id])
        return JsonResponse({'success': True})
    except Exception as e:
        return JsonResponse({'success': False, 'error': str(e)})


@require_http_methods(["POST"])
def get_contact_details(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in

    db = request.session.get('db_name', 'default')  # Use default if db_name is not in session

    data = json.loads(request.body)
    contact_id = data.get('contact_id')
    try:
        with connections[db].cursor() as cursor:
            cursor.execute("SELECT contact_id, patient_id, contact_number FROM contact_numbers WHERE contact_id = %s", [contact_id])
            row = cursor.fetchone()
        if row:
            contact_data = {
                'contact_id': row[0],
                'patient_id': row[1],
                'contact_number': row[2]
            }
            return JsonResponse({'success': True, 'contact': contact_data})
        else:
            return JsonResponse({'success': False, 'error': 'Contact not found'})
    except Exception as e:
        return JsonResponse({'success': False, 'error': str(e)})


@csrf_exempt
def process_query(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in

    db = request.session.get('db_name', 'default')  # Use default if db_name is not in session

    data = json.loads(request.body)
    nl_query = data['query']
    try:
        sql_query = translate_to_sql(nl_query)  # Function to translate NL to SQL
        results = execute_sql_query(sql_query, request)  # Function to execute SQL and return results
        print(results)
        return JsonResponse({'success': True, 'result': results})
    except Exception as e:
        return JsonResponse({'success': False, 'error': str(e)})
    

def messages_view(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in

    username = request.session.get('username')  # Make sure to use .get for safe access
    db = request.session.get('db_name')
    query = "SELECT message_id, appointment_id, patient_id, visit_id, date, type, content FROM messages"
    with connections[db].cursor() as cursor:  
        cursor.execute(query)
        rows = cursor.fetchall()

    messages = [
        {
            'message_id': row[0],
            'appointment_id': row[1],
            'patient_id': row[2],
            'visit_id': row[3],
            'date': row[4],
            'type': row[5],
            'content': row[6]
        } for row in rows
    ]
   
    return render(request, 'user/messages.html', {'messages': messages, 'username': username})


def add_message(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in
    
    if request.method == 'POST':
        message_id = request.POST.get('message_id')
        appointment_id = request.POST.get('appointment_id')
        patient_id = request.POST.get('patient_id')
        visit_id = request.POST.get('visit_id')
        date = request.POST.get('date')
        type_ = request.POST.get('type')
        content = request.POST.get('content')

        db = request.session.get('db_name', 'default')  # Use default if db_name is not in session

        query = "INSERT INTO messages (message_id, appointment_id, patient_id, visit_id, date, type, content) VALUES (%s, %s, %s, %s, %s, %s, %s)"
        params = [message_id, appointment_id, patient_id, visit_id, date, type_, content]

        try:
            with connections[db].cursor() as cursor:
                cursor.execute(query, params)
            return redirect('messages')  # Redirect if no error
        except IntegrityError:
            messages.error(request, "A message with the same ID already exists. Please use a different ID.")  # Add an error message

    # Always show the form if GET or if there was a POST error
    return render(request, 'user/addMessage.html')


@require_POST
@csrf_exempt
def delete_messages(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in

    db = request.session.get('db_name', 'default')  # Use default if db_name is not in session

    try:
        data = json.loads(request.body)
        message_ids = data.get('message_ids', [])

        # Use the 'db' database connection
        with connections[db].cursor() as cursor:
            # Construct and execute the SQL query for each message ID
            for message_id in message_ids:
                cursor.execute("DELETE FROM messages WHERE message_id = %s", [message_id])

        return JsonResponse({'success': True})
    except Exception as e:
        return JsonResponse({'success': False, 'error': str(e)})


@require_http_methods(["POST"])
def update_message(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in
    
    db = request.session.get('db_name', 'default')  # Use default if db_name is not in session

    data = json.loads(request.body)
    message_id = data.get('message_id')
    appointment_id = data.get('appointment_id')
    patient_id = data.get('patient_id')
    visit_id = data.get('visit_id')
    date = data.get('date')
    type_ = data.get('type')
    content = data.get('content')

    try:
        with connections[db].cursor() as cursor:
            cursor.execute("""
                UPDATE messages SET appointment_id = %s, patient_id = %s, visit_id = %s, date = %s, type = %s, content = %s
                WHERE message_id = %s
            """, [appointment_id, patient_id, visit_id, date, type_, content, message_id])
        return JsonResponse({'success': True})
    except Exception as e:
        return JsonResponse({'success': False, 'error': str(e)})


@require_http_methods(["POST"])
def get_message_details(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in

    db = request.session.get('db_name', 'default')  # Use default if db_name is not in session

    data = json.loads(request.body)
    message_id = data.get('message_id')
    try:
        with connections[db].cursor() as cursor:
            cursor.execute("SELECT message_id, appointment_id, patient_id, visit_id, date, type, content FROM messages WHERE message_id = %s", [message_id])
            row = cursor.fetchone()
        if row:
            message_data = {
                'message_id': row[0],
                'appointment_id': row[1],
                'patient_id': row[2],
                'visit_id': row[3],
                'date': row[4],
                'type': row[5],
                'content': row[6]
            }
            return JsonResponse({'success': True, 'message': message_data})
        else:
            return JsonResponse({'success': False, 'error': 'Message not found'})
    except Exception as e:
        return JsonResponse({'success': False, 'error': str(e)})

def appointments_view(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in

    username = request.session.get('username')  # Make sure to use .get for safe access
    db = request.session.get('db_name')
    query = "SELECT appointment_id, patient_id, date, time, purpose FROM appointments"
    with connections[db].cursor() as cursor:  
        cursor.execute(query)
        rows = cursor.fetchall()

    appointments = [
        {
            'appointment_id': row[0],
            'patient_id': row[1],
            'date': row[2],
            'time': row[3],
            'purpose': row[4]
        } for row in rows
    ]
   
    return render(request, 'user/appointments.html', {'appointments': appointments, 'username': username})


def add_appointment(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in
    
    if request.method == 'POST':
        appointment_id = request.POST.get('appointment_id')
        patient_id = request.POST.get('patient_id')
        date = request.POST.get('date')
        time = request.POST.get('time')
        purpose = request.POST.get('purpose')

        db = request.session.get('db_name', 'default')  # Use default if db_name is not in session

        query = "INSERT INTO appointments (appointment_id, patient_id, date, time, purpose) VALUES (%s, %s, %s, %s, %s)"
        params = [appointment_id, patient_id, date, time, purpose]

        try:
            with connections[db].cursor() as cursor:
                cursor.execute(query, params)
            return redirect('appointments')  # Redirect if no error
        except IntegrityError:
            messages.error(request, "An appointment with the same ID already exists. Please use a different ID.")  # Add an error message

    # Always show the form if GET or if there was a POST error
    return render(request, 'user/addAppointment.html')


@require_POST
@csrf_exempt
def delete_appointments(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in

    db = request.session.get('db_name', 'default')  # Use default if db_name is not in session

    try:
        data = json.loads(request.body)
        appointment_ids = data.get('appointment_ids', [])

        # Use the 'db' database connection
        with connections[db].cursor() as cursor:
            # Construct and execute the SQL query for each appointment ID
            for appointment_id in appointment_ids:
                cursor.execute("DELETE FROM appointments WHERE appointment_id = %s", [appointment_id])

        return JsonResponse({'success': True})
    except Exception as e:
        return JsonResponse({'success': False, 'error': str(e)})


@require_http_methods(["POST"])
def update_appointment(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in
    
    db = request.session.get('db_name', 'default')  # Use default if db_name is not in session

    data = json.loads(request.body)
    appointment_id = data.get('appointment_id')
    patient_id = data.get('patient_id')
    date = data.get('date')
    time = data.get('time')
    purpose = data.get('purpose')

    try:
        with connections[db].cursor() as cursor:
            cursor.execute("""
                UPDATE appointments SET patient_id = %s, date = %s, time = %s, purpose = %s
                WHERE appointment_id = %s
            """, [patient_id, date, time, purpose, appointment_id])
        return JsonResponse({'success': True})
    except Exception as e:
        return JsonResponse({'success': False, 'error': str(e)})


@require_http_methods(["POST"])
def get_appointment_details(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in

    db = request.session.get('db_name', 'default')  # Use default if db_name is not in session

    data = json.loads(request.body)
    appointment_id = data.get('appointment_id')
    try:
        with connections[db].cursor() as cursor:
            cursor.execute("SELECT appointment_id, patient_id, date, time, purpose FROM appointments WHERE appointment_id = %s", [appointment_id])
            row = cursor.fetchone()
        if row:
            appointment_data = {
                'appointment_id': row[0],
                'patient_id': row[1],
                'date': row[2],
                'time': row[3],
                'purpose': row[4]
            }
            return JsonResponse({'success': True, 'appointment': appointment_data})
        else:
            return JsonResponse({'success': False, 'error': 'Appointment not found'})
    except Exception as e:
        return JsonResponse({'success': False, 'error': str(e)})

def services_view(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in

    username = request.session.get('username')  # Make sure to use .get for safe access
    db = request.session.get('db_name')
    query = "SELECT service_id, name, description FROM services"
    with connections[db].cursor() as cursor:  
        cursor.execute(query)
        rows = cursor.fetchall()

    services = [
        {
            'service_id': row[0],
            'name': row[1],
            'description': row[2]
        } for row in rows
    ]
   
    return render(request, 'user/services.html', {'services': services, 'username': username})


def add_service(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in
    
    if request.method == 'POST':
        service_id = request.POST.get('service_id')
        name = request.POST.get('name')
        description = request.POST.get('description')

        db = request.session.get('db_name', 'default')  # Use default if db_name is not in session

        query = "INSERT INTO services (service_id, name, description) VALUES (%s, %s, %s)"
        params = [service_id, name, description]

        try:
            with connections[db].cursor() as cursor:
                cursor.execute(query, params)
            return redirect('services')  # Redirect if no error
        except IntegrityError:
            messages.error(request, "A service with the same ID already exists. Please use a different ID.")  # Add an error message

    # Always show the form if GET or if there was a POST error
    return render(request, 'user/addService.html')


@require_POST
@csrf_exempt
def delete_services(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in

    db = request.session.get('db_name', 'default')  # Use default if db_name is not in session

    try:
        data = json.loads(request.body)
        service_ids = data.get('service_ids', [])

        # Use the 'db' database connection
        with connections[db].cursor() as cursor:
            # Construct and execute the SQL query for each service ID
            for service_id in service_ids:
                cursor.execute("DELETE FROM services WHERE service_id = %s", [service_id])

        return JsonResponse({'success': True})
    except Exception as e:
        return JsonResponse({'success': False, 'error': str(e)})


@require_http_methods(["POST"])
def update_service(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in
    
    db = request.session.get('db_name', 'default')  # Use default if db_name is not in session

    data = json.loads(request.body)
    service_id = data.get('service_id')
    name = data.get('name')
    description = data.get('description')

    try:
        with connections[db].cursor() as cursor:
            cursor.execute("""
                UPDATE services SET name = %s, description = %s
                WHERE service_id = %s
            """, [name, description, service_id])
        return JsonResponse({'success': True})
    except Exception as e:
        return JsonResponse({'success': False, 'error': str(e)})


@require_http_methods(["POST"])
def get_service_details(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in

    db = request.session.get('db_name', 'default')  # Use default if db_name is not in session

    data = json.loads(request.body)
    service_id = data.get('service_id')
    try:
        with connections[db].cursor() as cursor:
            cursor.execute("SELECT service_id, name, description FROM services WHERE service_id = %s", [service_id])
            row = cursor.fetchone()
        if row:
            service_data = {
                'service_id': row[0],
                'name': row[1],
                'description': row[2]
            }
            return JsonResponse({'success': True, 'service': service_data})
        else:
            return JsonResponse({'success': False, 'error': 'Service not found'})
    except Exception as e:
        return JsonResponse({'success': False, 'error': str(e)})
    

def treatment_records_view(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in

    username = request.session.get('username')  # Make sure to use .get for safe access
    db = request.session.get('db_name')
    query = "SELECT record_id, visit_id, description FROM treatment_records"
    with connections[db].cursor() as cursor:  
        cursor.execute(query)
        rows = cursor.fetchall()

    treatment_records = [
        {
            'record_id': row[0],
            'visit_id': row[1],
            'description': row[2]
        } for row in rows
    ]
   
    return render(request, 'user/treatment_records.html', {'treatment_records': treatment_records, 'username': username})


def add_treatment_record(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in
    
    if request.method == 'POST':
        visit_id = request.POST.get('visit_id')
        description = request.POST.get('description')

        db = request.session.get('db_name', 'default')  # Use default if db_name is not in session

        query = "INSERT INTO treatment_records (visit_id, description) VALUES (%s, %s)"
        params = [visit_id, description]

        try:
            with connections[db].cursor() as cursor:
                cursor.execute(query, params)
            return redirect('treatment_records')  # Redirect if no error
        except IntegrityError:
            messages.error(request, "A record with the same ID already exists. Please use a different ID.")  # Add an error message

    return render(request, 'user/addTreatmentRecord.html')


@require_POST
@csrf_exempt
def delete_treatment_records(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in

    db = request.session.get('db_name', 'default')  # Use default if db_name is not in session

    try:
        data = json.loads(request.body)
        record_ids = data.get('record_ids', [])

        with connections[db].cursor() as cursor:
            for record_id in record_ids:
                cursor.execute("DELETE FROM treatment_records WHERE record_id = %s", [record_id])

        return JsonResponse({'success': True})
    except Exception as e:
        return JsonResponse({'success': False, 'error': str(e)})


@require_http_methods(["POST"])
def update_treatment_record(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in
    
    db = request.session.get('db_name', 'default')  # Use default if db_name is not in session

    data = json.loads(request.body)
    old_visit_id = data.get('old_visit_id')
    new_visit_id = data.get('visit_id')
    description = data.get('description')

    try:
        with connections[db].cursor() as cursor:
            cursor.execute("""
                UPDATE treatment_records SET visit_id = %s, description = %s
                WHERE visit_id = %s
            """, [new_visit_id, description, old_visit_id])
        return JsonResponse({'success': True})
    except Exception as e:
        return JsonResponse({'success': False, 'error': str(e)})


@require_http_methods(["POST"])
def get_treatment_record_details(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in

    db = request.session.get('db_name', 'default')  # Use default if db_name is not in session

    data = json.loads(request.body)
    record_id = data.get('record_id')
    try:
        with connections[db].cursor() as cursor:
            cursor.execute("SELECT visit_id, description FROM treatment_records WHERE record_id = %s", [record_id])
            row = cursor.fetchone()
        if row:
            record_data = {
                'visit_id': row[0],
                'description': row[1]
            }
            return JsonResponse({'success': True, 'record': record_data})
        else:
            return JsonResponse({'success': False, 'error': 'Record not found'})
    except Exception as e:
        return JsonResponse({'success': False, 'error': str(e)})



    

def visits_view(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in

    username = request.session.get('username')  # Make sure to use .get for safe access
    db = request.session.get('db_name')
    query = "SELECT patient_id, visit_id, date, time FROM visits"
    with connections[db].cursor() as cursor:  
        cursor.execute(query)
        rows = cursor.fetchall()

    visits = [
        {
            'patient_id': row[0],
            'visit_id': row[1],
            'date': row[2],
            'time': row[3]
        } for row in rows
    ]
   
    return render(request, 'user/visits.html', {'visits': visits, 'username': username})


def add_visit(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in
    
    if request.method == 'POST':
        patient_id = request.POST.get('patient_id')
        visit_id = request.POST.get('visit_id')
        date = request.POST.get('date')
        time = request.POST.get('time')

        db = request.session.get('db_name', 'default')  # Use default if db_name is not in session

        query = "INSERT INTO visits (patient_id, visit_id, date, time) VALUES (%s, %s, %s, %s)"
        params = [patient_id, visit_id, date, time]

        try:
            with connections[db].cursor() as cursor:
                cursor.execute(query, params)
            return redirect('visits')  # Redirect if no error
        except IntegrityError:
            messages.error(request, "A visit with the same ID already exists. Please use a different ID.")  # Add an error message

    # Always show the form if GET or if there was a POST error
    return render(request, 'user/addVisit.html')


@require_POST
@csrf_exempt
def delete_visits(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in

    db = request.session.get('db_name', 'default')  # Use default if db_name is not in session

    try:
        data = json.loads(request.body)
        visit_ids = data.get('visit_ids', [])

        # Use the 'db' database connection
        with connections[db].cursor() as cursor:
            # Construct and execute the SQL query for each visit ID
            for visit_id in visit_ids:
                cursor.execute("DELETE FROM visits WHERE visit_id = %s", [visit_id])

        return JsonResponse({'success': True})
    except Exception as e:
        return JsonResponse({'success': False, 'error': str(e)})


@require_http_methods(["POST"])
def update_visit(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in
    
    db = request.session.get('db_name', 'default')  # Use default if db_name is not in session

    data = json.loads(request.body)
    patient_id = data.get('patient_id')
    visit_id = data.get('visit_id')
    date = data.get('date')
    time = data.get('time')

    try:
        with connections[db].cursor() as cursor:
            cursor.execute("""
                UPDATE visits SET patient_id = %s, date = %s, time = %s
                WHERE visit_id = %s
            """, [patient_id, date, time, visit_id])
        return JsonResponse({'success': True})
    except Exception as e:
        return JsonResponse({'success': False, 'error': str(e)})


@require_http_methods(["POST"])
def get_visit_details(request):
    if not request.session.get('is_logged_in', False):
        return redirect('login')  # Redirect to login if not logged in

    db = request.session.get('db_name', 'default')  # Use default if db_name is not in session

    data = json.loads(request.body)
    visit_id = data.get('visit_id')
    try:
        with connections[db].cursor() as cursor:
            cursor.execute("SELECT visit_id, patient_id, date, time FROM visits WHERE visit_id = %s", [visit_id])
            row = cursor.fetchone()
        if row:
            visit_data = {
                'visit_id': row[0],
                'patient_id': row[1],
                'date': row[2],
                'time': row[3]
            }
            return JsonResponse({'success': True, 'visit': visit_data})
        else:
            return JsonResponse({'success': False, 'error': 'Visit not found'})
    except Exception as e:
        return JsonResponse({'success': False, 'error': str(e)})