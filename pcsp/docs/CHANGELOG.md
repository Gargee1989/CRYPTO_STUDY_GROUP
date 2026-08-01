# DOCUMENT CHANGES MADE BY INDIVIDUALS FOR IMPLEMENTED CODE:

Details regarding implementation and changes of certain parts of the code. Incomplete, Todo, better implementation expectation can be also logged. 



### REQ-CIP-402 : nonQualities/Ronit_Choudhury:
  -  All test cases handled except non-alphabatic keys.
  -  Needs better error handling.

### REQ-CIP-501 : vedbd/Ved_Bhandary
  - All test cases handled except `encrypt_rejects_non_alpha_key`.
  - Error handling is still an issue. 

### REQ-CIP-502 : vedbd/Ved_Bhandary
  - All test cases handled.
  - IDEA: Central logger of sorts?

### REQ-CIP-701 : vedbd/Ved_Bhandary
  - Edge cases handled.
  - Custom test cases passed.
  - Code cleanup is required.
  -  
### REQ-CIP-401 : Saurav_Chakraborty
  - Implemented the Caesar cipher with one-byte key validation and modulo-26 shifts.
  - Added encryption, decryption, wraparound, round-trip, empty-input, and invalid-key tests.

### REQ-CIP-403 : Saurav_Chakraborty
  - Implemented the keyless Atbash cipher over the 0..25 alphabet domain.
  - Added encryption, decryption, round-trip, empty-input, and keyless behavior tests.

### REQ-PRT-203: Ronit_Choudhury
- Parser implemented with AI
- Will do a redundancy check soon