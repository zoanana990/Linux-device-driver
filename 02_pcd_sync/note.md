# Synchronization

What is race condition?

- A race condition is a runtime phenomenon where 2 or more threads of execution race towards a shared resource to access it. 
  This may result in an inconsistent state of the shared resources and can cause a software bug

What is shared resousrces?

- A shoared resource is a resource such as a global variable, a shared memory area, 
  a peripheral that can be accessed by multipple threads of execution on the same processor or different processor

What is critical section?

- Code block which manipulates the shared resource is known as a critical section.
  Care must be taken on such code blocks to protect them from concurrent access from 2 or more threads of execution 
  because there could be an attempt of concurrent access which leads to race condition

- Critical section code must be atomic. That means, until one thread of execution completes the execution of a critical section,
  no other thread of execution is allowed to enter the same critical section

- Use locks to make a critical section atomic


