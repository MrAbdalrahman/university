from django.core.management.base import BaseCommand
from django.db import connections
from mainApp.models import UserDatabase

class Command(BaseCommand):
    help = 'List all user databases and their tables'

    def handle(self, *args, **kwargs):
        databases = UserDatabase.objects.all()
        for db in databases:
            alias = db.db_name
            self.stdout.write(f"Database: {alias}")
            with connections[alias].cursor() as cursor:
                cursor.execute("SHOW TABLES")
                tables = cursor.fetchall()
                if tables:
                    for table in tables:
                        self.stdout.write(f" - {table[0]}")
                else:
                    self.stdout.write(" - No tables found.")
            self.stdout.write("\n")