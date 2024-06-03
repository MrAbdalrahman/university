from django.contrib.auth.signals import user_logged_in
from django.dispatch import receiver
from django.conf import settings
import copy
from django.db import connections 

@receiver(user_logged_in)
def set_user_database(sender, user, request, **kwargs):
    db_name = f"clinic_user_{user.username}"
    
    # Create a deep copy of the current DATABASES settings
    new_databases = copy.deepcopy(settings.DATABASES)

    # Add the new database configuration
    new_databases[db_name] = {
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

    # Update settings with the new database configuration
    settings.DATABASES.update(new_databases)
    connections.databases = new_databases  # Ensure the new database configuration is loaded
