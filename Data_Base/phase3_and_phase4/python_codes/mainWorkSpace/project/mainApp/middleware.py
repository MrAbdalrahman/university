from django.utils.deprecation import MiddlewareMixin
from django.conf import settings
import copy
from django.db import connections 
import json

class SetDatabaseMiddleware(MiddlewareMixin):
    def process_request(self, request):
        if request.user.is_authenticated:
            db_name = f"clinic_user_{request.user.username}"

            # load more database configurations from the JSON file
            with open('db_configs.json', 'r') as f:
                db_configs = json.load(f)

            if db_name in db_configs:
                new_databases = copy.deepcopy(settings.DATABASES)
                new_databases[db_name] = db_configs[db_name]
                settings.DATABASES.update(new_databases)
            connections.databases = new_databases