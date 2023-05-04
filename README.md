# **BDII Proyecto 1** #


## **Tabla de contenidos**
* Descripcion General
* Tecnicas de organizacion de archivos
* Setup

### **1. SEQUENTIAL FILE**

### Insercion
Para cada registro nuevo se verifica que no esten en el archivo llamando a al metodo *search()* . Se hace busqueda del maximo elemento menor en el archivo principal conplejidad *O(log(n))* y tambien en el archivo auxliar Complejidad *O(k)*. Se hace el intercambio de punteros. Y se inserta en al archivo auxliar. Complejidad O(1).

_**Para llamar a la funcion insercion:**_
```
bool add(Rtitles new_record){}
```


#### Busqueda
Ya que cada registro es de lonngitud variable se opta por incorporar un archivo metadata al archivo principal con la ubicacion fisica de cada registro. De acuerdo al metadata se recorre el archivo principal aplicando el algoritmo regular de busqueda binaria. Si no se encuntra el archivo, se busca en el archivo auxiliar con busqueda lineal.

_**Para llamar a la funcion busqueda:**_
```
Rtitles search(string key){}
```


#### Eliminacion
Se ubica el ultimo registro registro anterior y se hace intercambio de punteros caso contrario solo se actuliza el estado del registro a -1. Es decir se aplica la estrategia del marcado.

_**Para llamar a la funcion eliminar:**_
```
bool remove(string key){}
```

#### Busqueda por rango
Aprovechando el soporte de busqueda por rango de la tecnica. Se ubica el primer elemento menor en el archivo principal o axuliar y se lee los registros siguientes, siguiendo los punteros.


_**Para llamar a la funcion busqueda por rangos:**_
```
vector<Rtitles> rangeSearch(string begin_key, string end_key){}
```


#### Recontruccion
```
Rtitles search(string key){}
```


### **2. HASH FILE**

