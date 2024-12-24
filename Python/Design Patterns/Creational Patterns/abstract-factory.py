'''
Abstract Factory Design mentions -
1. Create Interfaces for each distinct product of the product family (e.g. chair, sofa, coffe table)
2. Make all variants of products follow those interfaces. e.g. all chair variants will implement chair interface, all coffe table variants will implement the coffe table interface.
3. Next we have to declare the `Abstract Factory` - an interface for creating products that are part of the same family, abstract factory must return the abstract class of each distinct product family (e.g. chair, sofa, coffe table)

Articles: 
https://refactoring.guru/design-patterns/abstract-factory
'''

# Chair Interface
from abc import ABC, abstractmethod
class Chair(ABC):
    @abstractmethod
    def sitOn(self):
        pass
    
    @abstractmethod
    def hasLegs(self):
        pass

class Sofa(ABC):
    @abstractmethod
    def sitOn(self) -> None:
        pass
    @abstractmethod
    def hasLegs(self) -> None:
        pass

# Victorian Chair
class VictorianChair(Chair):
    def sitOn(self):
        print("Sit on the victorian chair.")
        
    def hasLegs(self):
        print("I have 4 legs")
        
class VictorianSofa(Sofa):
    def sitOn(self) -> None:
        print("Sitting on victorian sofa.")
        
    def hasLegs(self):
        print("Victorian sofa has 4 legs.")
        
# Abstract Factory
class FurnitureFactory(ABC):
    @abstractmethod
    def createChair(self) -> Chair:
        pass
    
    def createSofa(self) -> Sofa:
        pass

# Class implementing the product factory of each variant
class VictorianFurnitureFactory(FurnitureFactory):
    def createChair(self) -> VictorianChair:
        chair = VictorianChair()
        return chair
    
    def createSofa(self) -> VictorianSofa:
        sofa = VictorianSofa()
        return sofa