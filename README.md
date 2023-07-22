# Passwdgen - terminal password generation utlity

## Usage
```bash
$ passwdgen
count=5, len=8, alphabet=ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789
tMO0iKcE
wmmD9T38
GxtJ4yas
uJKsjIVz
we8esJsT
```
```bash
$ passwdgen --without-header
tMO0iKcE
wmmD9T38
GxtJ4yas
uJKsjIVz
we8esJsT
```
```bash
$ passwdgen -l10 -c1 --without-header
meuYJFASju
```

## Options
| Option              	| Value      	| Description                             	|
|---------------------	|------------	|-----------------------------------------	|
| -l,--len            	| number, >0 	| Length of generated password            	|
| -c,--count          	| number, >0 	| Count of password variants              	|
| -a,--alphabet       	| string     	| Char sequence used by generate password 	|
| -r,--human-readable 	| none       	| Put "-" between every four char.        	|
| --seed0,            	| number     	| Use used defined seed for randomizer    	|
| --seed1              	| number     	| Use used defined seed for randomizer    	|
| --without-header    	| none       	| Dont print header information           	|
| -h,--help           	| none       	| Print help                              	|
| -v,--version        	| none       	| Print version                           	|

## Build
```bash
$ make && make install
```