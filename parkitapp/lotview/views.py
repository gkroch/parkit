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
    nodeID = data.__getitem__('node')
    direct = data.__getitem__('direction')

    # Create sensor event
    tempnode = get_object_or_404(Node, pk=nodeID)
    templot = tempnode.lot
    sensorevent = SensorEvent(timestamp=time,node=tempnode,
                              direction=direct)
    sensorevent.save()

    # Update database (TODO replace with update function)
    if direct == 'e':
        tempnode.net_change -= 1
        tempnode.save()
        templot.spots_remaining -= 1
        templot.save()
    elif direct == 'x':
        tempnode.net_change += 1
        tempnode.save()
        templot.spots_remaining += 1
        templot.save()

    context = {
        'timestamp'   : time,
        'direction'   : direct,
        'node'        : tempnode.net_change,
        'lot'         : templot.spots_remaining
    }

    response = JsonResponse(context)

    return response
    # return render(request, 'lotview/data.html', context)
