## Cache resuts of solve(mat) [solve() calculates inverse of a matrix]. 
## Caching is helpfull to preserve resources when a similar job is being re executed. Insted of re computing we can
## return the saved result.

## This function creates a special "matrix" object that can cache its inverse

makeCacheMatrix <- function(x = matrix()) {
      inv <- NULL
      set <- function(y) {
            x <<- y
            inv <<-NULL
      }
      get <- function() x
      setinverse <- function(inverse) inv <<- inverse ## This will set inv global to the enviroment
      getinverse <- function() inv
      list(set = set, get = get, setinverse = setinverse, getinverse=getinverse) #This returns a list with appropraiate 
      #labels so that they can be easily called from the cacheSolve function using '$' symbol.
}


## This function computes the inverse of the special "matrix" returned by makeCacheMatrix above.
## If the inverse has already been calculated (and the matrix has not changed), then the cachesolve retrieves 
## the inverse from the cache.

cacheSolve <- function(x, ...) {
        ## Return a matrix that is the inverse of 'x'
      inv <- x$getinverse()
      if(!is.null(inv)){
            message("getting cached data..")
            return(inv)
            }
      data <- x$get()
      inv <- solve(data, ...)
      x$setinverse(inv)
      inv
}
