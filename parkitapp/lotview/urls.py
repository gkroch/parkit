from django.conf.urls import url
from . import views

urlpatterns = [
    url(r'^$', views.index, name='index'),
    url(r'^data/(?P<nodeID>[0-9])/(?P<direct>[e,x,u])/$',
        views.data, name='data')
    ]
