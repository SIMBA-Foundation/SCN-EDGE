# System Deployment of SCN Edge Node

## Content
Introduce how to deploy SCN edge node system by steps

## Front-end Requirement
Insert the information of edge cluster in database of control center (contact system Admin for this step)

Insert node information. Following is an example of inserting three nodes.

```sql
insert into edge_cluster (id, clusterid, host, http_port, state, create_time, update_time, last_start_time, rpc_port)
values 
(1, 1, 'xxx.xxx.xxx.xxx', 443, '', cast(extract(epoch from now()) as bigint), cast(extract(epoch from now()) as bigint), 0, 9001), 
(2, 2, 'xxx.xxx.xxx.xxx', 443, '', cast(extract(epoch from now()) as bigint), cast(extract(epoch from now()) as bigint), 0, 9001),
(3, 3, 'xxx.xxx.xxx.xxx', 443, '', cast(extract(epoch from now()) as bigint), cast(extract(epoch from now()) as bigint), 0, 9001);
``` 


## Login Docker

* Log in

```shell
docker login xxx.xxx.xxx.xxx
username: your_docker_registry_username
password: ******
```
* Docker push could be processed now

* Configure nfs service. Since we need the openwhisk system and it needs to be stored for a long period of time, so let’s configure some nfs directories in advance in case they are needed.

* Install nfs (how to install nfs would not be repeated here)
* Establish the following directory on nfs server

```
# prefix /var/k8s_volumes can be adjusted on your own
```

* Set up nfs to open the following directory

```shell
# Please note that the ip segment needs to be set to be an intranet ip segment of your own worker, which needs to be correspondent to the prefix /var/k8s_volumes
/var/k8s_volumes/openwhisk xxx.xxx.xxx.0/24(rw,no_subtree_check,no_root_squash)
```

* nfs volume is now available for access 

## Install openwhisk

* Please refer to the document kubernetes、openwhisk system deployment (https://github.com/apache/incubator-openwhisk-deploy-kube)
* Please use the configured nfs to support the persistent of openwhisk
* One action needs to be set up after the installation of openwhisk. The action will be used to create users afterwards.
* After the installment of openwhisk, a default guest account is created. We can configure wsk with apikey from the guest account.
```shell
wsk -i property set --apihost ${openwhisk_ip}:31001
wsk -i property set --auth '23bc46b1-71f6-4ed5-8c54-816aa4f8c502:123zO3xZCLrMN6v2BKK1dXYFpXlPkccOFqm12CdAsMgRU4VrNZ9lyGVCGuMDGIwP'
```
${openwhisk ip} can be replaced by any ip address of the worker’s

* Execute the command of creating the action, please use the following attachment user_create.py
```shell
wsk -i action update user_create user_create.py --kind python:3 --web false
```

## Configure docker registry trust 

* Currently we are using self-build docker image registry with self-signed certification, which needs to configure trust on the request docker
* Set up the following directory, replace ${host_ip_or_hostname} with self-signed registry ip or domain name.
* The current intranet address for the self-signed docker registry is internal_ip, address for public network is public_ip

```shell
/etc/docker/certs.d/public_ip
```

* Put ca.crt from self-signed docker registry into this directory. Please use your self_docker_registry_ca.crt from the attachment and remember to rename as ca.crt (contact system administrator to get the get the ca.crt)

##Restart docker
* Restart docker

```shell
systemctl restart docker
```

* Test. To verify if the configuration is successful, we do docker login on every machine

```shell
docker login public_ip
username: XXX
password: ******
```

## Start configuring the secret used by namespace & pull image
### Create namespace
* For now, we just use one namespace, which is openwhisk, in the edge node system
* Related namespace has been created during the process of openwhisk installation above. No more operation is needed here.
 
### Configure the pull secret of namespace
* Execute the following command

```shell
kubectl -nopenwhisk create secret docker-registry qingzhou-registry \
    --docker-server=public_ip \
    --docker-username=xxx \
    --docker-password=******
```

## Start installation FDN

TODO ...