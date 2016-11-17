from django.shortcuts import render
from django.http import HttpResponse
from django.views.decorators.http import require_http_methods
import datetime

from .models import SensorEvent, Node, Lot

# Create your views here.

def index(request):

    l = Lot.objects.get(pk=1)
    
    context = {'spots_remaining' : l.spots_remaining}

    return render(request, 'lotview/index.html', context)

@require_http_methods(["POST","GET"])
def data(request, nodeID, direct):
    time = datetime.datetime.now() # Get current time

    # Create sensor event
    tempnode = Node.objects.get(pk=nodeID)
    sensorevent = SensorEvent(timestamp=time,node=tempnode,direction=direct)
    sensorevent.save()

    context = {
        'sensorevent' : sensorevent,
        'timestamp'   : time,
        'direction'   : direct,
    }

    return render(request, 'lotview/data.html', context)
