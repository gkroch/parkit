from django.db import models

# Create your models here.

class Lot(models.Model):
    name = models.CharField(max_length=50)
    spots_remaining = models.IntegerField()

    def __str__(self):
        return self.name

class Node(models.Model):
    location = models.CharField(max_length=50)
    lot = models.ForeignKey(Lot)
    net_change = models.IntegerField()

    
    def __str__(self):
        return self.location

class SensorEvent(models.Model):
    timestamp = models.DateTimeField('Timestamp')
    node = models.ForeignKey(Node)

    # Positive for exit, negative for entry
    net_change = models.IntegerField()

    def __str__(self):
        return str(self.node) + " " + str(self.timestamp) + " " + \
            str(self.net_change)
    
