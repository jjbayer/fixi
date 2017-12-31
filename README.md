# fixi
Interpreted stack-oriented postfix language

## Examples

### Simple math

```
>>> 1 2 +
3 
```
### Defining functions

```
>>> { dup * } :square =

>>> 3 square        
9
```
## TODO

- [ ] Correctly tokenize curly braces
- [ ] conditionals, loops 
