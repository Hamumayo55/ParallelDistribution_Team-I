require 'benchmark'

result = Benchmark.realtime do
  #処理
  def midPoint(nums)
  	return ((nums.count / 2) + 0.5).floor
  end

  def setMerge(arr1 , arr2)
  	arr = []
  	while arr1.count > 0 || arr2.count > 0 do
  		if arr1.count == 0 then
  			arr.push(arr2.shift)
  		elsif arr2.count == 0 then
  			arr.push(arr1.shift)
  		elsif arr1[0] > arr2[0] then
  			arr.push(arr2.shift)
  		else
  			arr.push(arr1.shift)
  		end
  	end
  	return arr
  end

  def mergeSort(numbers)
  	if numbers.count == 0 then
  		numbers = []
  	elsif numbers.count == 1 then
  		numbers = numbers
  	elsif numbers.count == 2 then
  		if numbers[0] > numbers[1] then
  			tmp = numbers[0]
  			numbers[0] = numbers[1]
  			numbers[1] = tmp
  		end
  	else
  		midNum = midPoint(numbers)
  		arr1 = numbers.slice(0,midNum+1)
  		arr2 = numbers.slice(midNum+1,numbers.count)
  		arr1 = mergeSort(arr1)
  		arr2 = mergeSort(arr2)
  		numbers = setMerge(arr1 , arr2)
  	end
  	return numbers
  end

  numbers = 1000000.times.map{rand(10000000)}
end
puts "処理時間 #{result}s"
