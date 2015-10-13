stlc
----

This is an optimised type inference algorithm for the simply-typed (or
'monomorphic') λ-calculus.

It is quite straightforward to write a naïve implementation of this algorithm,
but having done so before in Haskell I was unhappy with the inefficiency of the
'literal' approach in C, allocating a struct for each term and each type, etc.
And so I strove to optimise the algorithm as best I could.

The result may not be as optimised as it could possibly be, but as it stands the
algorithm will, given a λ-term of size *n*, infer its simple typing in linear
time using only *n* words of overhead. (The naïve approach would use thrice the
space for each subterm, and thrice again for each type atom and operator.)

### Terms

Our term representation is an array of words, with the first (index 0) being the
length of the whole array. The rest of the array is structured as follows,
beginning with the 'root' at index 1.

* An abstraction at index 1, written *t₁ = λx₁.t₂*, is represented as a word of
value 1 (i.e. its own index), followed by its body subterm *t₂* at index 2.

* An application at index 1, written *t₁ = tₑt₂*, is represented as a word with
the index of its left-hand subterm *tₑ* as its value, followed by its right-hand
subterm *t₂* at index 2.

* A variable at index 1, written *t₁ = xₑ*, is represented as a word with the
*negative* of the index of its binder *tₑ* as its value, i.e. *-e*. This is
resemblant of the 'Bourbaki notation' for λ-terms.

There are also certain restrictions in place, such as the rejection of dead code
or the crisscrossing of applications' left-hand subterm positions. These can be
seen by reading the code used to validate such terms in `valid.c`.

### Types

The main trick used in this implementation is that the term representation is
*also* the primary way we represent types. A word that represents an abstraction
or application subterm *t₁* simultaneously represents a function type *τ₁*,
although this type is not necessarily the type of the corresponding term — the
bit pattern merely serves two purposes.

* An abstraction *t₁* also represents the type *τ₁ = α₁ → type(t₂)*.
* An application *t₁* also represents the type *τ₁ = type(t₂) → α₁*.

Two things must be introduced here. First, an array of *atoms*, the same size as
the original term, which at the beginning of the algorithm has been zeroed by
`calloc`. The atomic types *α₁* in the type representations above are indices
into this array. In fact, we distinguish between the function types *τₑ* and
atomic types *αₑ* by having the latter the negative of the former, i.e. *-e*.

Second, we define a mapping from terms to types, which should not be confused
with the type *representations* above.

* An abstraction *t₁* has the type *τ₁* (which is indeed its own type
representation).
* An application *t₁* has the type *α₁*, its corresponding type atom.
* A variable *t₁* has the type *αₑ*, the type atom corresponding to its binder.

### Unification

Now, the fact that the term representation doubles as a type representation
means that the only thing left to do is to unify those types which would need to
be unified in (the simple type fragment of) Damas and Milner's *Algorithm W*.
This type unification is the purpose of the atoms array: we use a variant of the
union-find algorithm, with path compression, to unify type atoms with each other
and with the function types.

Thus, for each application *t₁ = tₑt₂*, we must use type unification to unify
*τ₁* with *type(tₑ)*. We can do this by seeking through the term array and
attempting this unification at each application; no recursion over the term
structure is required. If at any point this unification fails then the term must
not be simply typed.

### Example

The term *λx₁.λx₂.x₁x₂* is represented by the data sequence `6 1 2 5 -2 -1`. The
components of this term are broken down as follows:

1. *t₁ = λx₁.t₂ : τ₁*; *τ₁ = α₁ → type(t₂)*.
2. *t₂ = λx₂.t₃ : τ₂*; *τ₂ = α₂ → type(t₃)*.
3. *t₃ = t₅t₄ : α₃*; *τ₃ = type(t₄) → α₃*.
4. *t₄ = x₂ : α₂*.
5. *t₅ = x₁ : α₁*.

However, at this stage the term is reckoned to have the type *α₁ → α₂ → α₃*,
which is not correct. This is rectified once we make a pass over the term and,
upon discovering the application at index 3, unify *τ₃* with *type(t₅)*. The
result is then, as expected, *(α₂ → α₃) → α₂ → α₃*. The data sequence of the
atom array at the end of this is simply `0 3 0 0 0 0`, where the value 3 at
index 1 indicates that the value of *α₁* has been set to *τ₃*.
