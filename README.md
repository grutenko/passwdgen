# Passwdgen - terminal password generation utlity

## Usage
```bash
$ passwdgen
tMO0iKcE
wmmD9T38
GxtJ4yas
uJKsjIVz
we8esJsT
```
```bash
$ passwdgen -l10 -c1
meuYJFASju
```

## Options
| Option              	| Value      	| Description                             	|
|---------------------	|------------	|-----------------------------------------	|
| -l,--len            	| number, >0 	| Length of generated password            	|
| -c,--count          	| number, >0 	| Count of password variants              	|
| -a,--alphabet       	| string     	| Char sequence used by generate password 	|
| -h,--help           	| none       	| Print help                              	|
| -v,--version        	| none       	| Print version                           	|

## Build
```bash
$ make && make install
```