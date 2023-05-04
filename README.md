
[![](https://img.shields.io/badge/github-blue?style=for-the-badge)](https://github.com/hamzamohdzubair/redant)
[![](https://img.shields.io/badge/book-blueviolet?style=for-the-badge)](https://hamzamohdzubair.github.io/redant/)
[![](https://img.shields.io/badge/API-yellow?style=for-the-badge)](https://docs.rs/crate/redant/latest)
# **$${\color{White} BD \space II \space PROYECTO \space I }$$** 
Primer proyecto de Base de Datos II


## **Tabla de contenidos**
* Descripcion General
* Tecnicas de organizacion de archivos
* Instalacion




### **1. SEQUENTIAL FILE**

### **Inserción**
> Para cada registro nuevo se verifica que no esten en el archivo llamando a al metodo *search()* . Se hace busqueda del maximo elemento menor en el archivo principal conplejidad *O(log(n))* y tambien en el archivo auxliar Complejidad *O(k)*. Se hace el intercambio de punteros. Y se inserta en al archivo auxliar. Complejidad O(1).

>__Note__ _**Para llamar a la funcion insercion:**_
```
bool add(Rtitles new_record){}
```


### **Búsqueda**
>Ya que cada registro es de lonngitud variable se opta por incorporar un archivo metadata al archivo principal con la ubicacion fisica de cada registro. De acuerdo al metadata se recorre el archivo principal aplicando el algoritmo regular de busqueda binaria. Si no se encuntra el archivo, se busca en el archivo auxiliar con busqueda lineal.

>__Note__ _**Para llamar a la funcion busqueda:**_
```
Rtitles search(string key){}
```


### **Eliminación**
>Se ubica el ultimo registro registro anterior y se hace intercambio de punteros caso contrario solo se actuliza el estado del registro a -1. Es decir se aplica la estrategia del marcado.

>__Note__ _**Para llamar a la funcion eliminar:**_
```
bool remove(string key){}
```

### **Busqueda por rango**
> Aprovechando el soporte de busqueda por rango de la tecnica. Se ubica el primer elemento menor en el archivo principal o axuliar y se lee los registros siguientes, siguiendo los punteros.


>__Note__ _**Para llamar a la funcion busqueda por rangos:**_
```
vector<Rtitles> rangeSearch(string begin_key, string end_key){}
```


### **Recontruccion**
```
Rtitles search(string key){}
```


### **2. EXTENDIBLE HASH FILE**

### **Inserción**


### **Búsqueda**


### **Eliminación**



