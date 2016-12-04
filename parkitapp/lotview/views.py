from django.shortcuts import render, get_object_or_404
from django.http import HttpResponse, HttpRequest, JsonResponse
from django.views.decorators.http import require_http_methods
from django.views.decorators.csrf import csrf_exempt
import datetime

from .models import SensorEvent, Node, Lot

# Create your views here.

def index(request):

    l = Lot.objects.get(pk=1)
    
    context = {'spots_remaining' : l.spots_remaining}

    return render(request, 'lotview/index.html', context)


@require_http_methods(["POST"])
@csrf_exempt
def data(request):
    time = datetime.datetime.now() # Get current time

    # Get data from POST request
    data = request.POST
    nodeID = int(data.__getitem__('node'))
    net = int(data.__getitem__('net'))

    # Create sensor event
    tempnode = get_object_or_404(Node, pk=nodeID)
    templot = tempnode.lot
    sensorevent = SensorEvent(timestamp=time,node=tempnode,
                              net_change=net)
    sensorevent.save()

    # Update database
    tempnode.net_change += sensorevent.net_change
    tempnode.save()
    templot.spots_remaining += sensorevent.net_change
    templot.save()
    
    context = {
        'timestamp'   : time,
        'sensor_net'  : net,
        'node_change' : tempnode.net_change,
        'lot_total'   : templot.spots_remaining
    }

    response = JsonResponse(context)

    return response
