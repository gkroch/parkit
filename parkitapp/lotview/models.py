from django.db import models

# Create your models here.

class SensorEvent(models.Model):
    timestamp = models.DateTimeField('Timestamp')
    node = models.IntegerField('Origin of signal')
    direction = models.CharField(max_length=10) # Entry / Exit / Unknown
