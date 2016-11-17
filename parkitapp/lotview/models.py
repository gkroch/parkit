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

    # Net change of entries and exits via node
    def net_change():
        pass

    def __str__(self):
        return self.location

class SensorEvent(models.Model):
    DIRECTIONS = (
        ('e','Entry'),
        ('x','Exit'),
        ('u','Unknown'),
        )
    
    timestamp = models.DateTimeField('Timestamp')
    node = models.ForeignKey(Node)
    direction = models.CharField(max_length=1, choices=DIRECTIONS) 

    def __str__(self):
        return str(self.node) + " " + str(self.timestamp) + " " + \
            str(self.direction)
    
