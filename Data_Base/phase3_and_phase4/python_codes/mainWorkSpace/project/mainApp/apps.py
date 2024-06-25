from django.apps import AppConfig
import json
from django.conf import settings
import copy

def load_db_configs():
    try:
        with open('db_configs.json', 'r') as f:
            db_configs = json.load(f)
        new_databases = copy.deepcopy(settings.DATABASES)
        new_databases.update(db_configs)
        settings.DATABASES.update(new_databases)
    except FileNotFoundError:
        pass  # No dynamic databases to load
    
class MainappConfig(AppConfig):
    default_auto_field = 'django.db.models.BigAutoField'
    name = 'mainApp'

    def ready(self):
         load_db_configs()
