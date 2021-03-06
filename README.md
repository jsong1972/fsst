# fsst
A library implementing FSST compression algorithm

## Performance
### Baseline
Encode: 3.28M/s, Decode: 2243.75M/s
```
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  ms/call  ms/call  name    
 53.95     16.75    16.75 1077785354     0.00     0.00  fsst::SymbolTable::StartsWith_(unsigned char const*, unsigned long, unsigned short)
 43.90     30.38    13.63 155724879     0.00     0.00  fsst::SymbolTable::FindLongestSymbol(unsigned char const*, unsigned long)
```

### Index to symbol table
Encode: 18.95M/s, Decode: 2639.21M/s

```
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  ms/call  ms/call  name    
 37.72      0.89     0.89 1018130360     0.00     0.00  fsst::SymbolTable::StartsWith_(unsigned char const*, unsigned long, unsigned short)
 33.90      1.69     0.80 156112557     0.00     0.00  fsst::SymbolTable::FindLongestSymbol(unsigned char const*, unsigned long)
```

### Use 64-bit array as the dictionary of encoding
Encode: 139.35M/s, Decode: 2757.82M/s
```
  %   cumulative   self              self     total
 time   seconds   seconds    calls  ms/call  ms/call  name
 92.68      2.15     2.15                             fsst::Encode(unsigned char*, unsigned long, unsigned char*, fsst::SymbolTable&)
  6.47      2.30     0.15                             fsst::Decode(unsigned char*, unsigned long, unsigned char*, unsigned long*, unsigned char*)
```
