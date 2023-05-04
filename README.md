# **BDII Proyecto 1** #


## **Tabla de contenidos**
* Descripcion General
* Tecnicas de organizacion de archivos
* Setup

### **1. SEQUENTIAL FILE**

### Insercion
Para cada registro nuevo se verifica que no esten en el archivo llamando a al metodo * search() * . Se hace busqueda del maximo elemento menor en el archivo principal conplejidad * O(log(n)) * y tambien en el archivo auxliar Complejidad * O(k) *. Se hace el intercambio de punteros. Y se inserta en al auxliar file. Complejidad O(1).

_**Para llamar a la funcion insercion: **_
```
bool add(Rtitles new_record){}
```


#### Busqueda
```
Rtitles search(string key){}
```


#### Eliminacion
```
Rtitles search(string key){}
```

#### Busqueda por rango
```
Rtitles search(string key){}
```


#### Recontruccion
```
Rtitles search(string key){}
```


### **2. HASH FILE**

