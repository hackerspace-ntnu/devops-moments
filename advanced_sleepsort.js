/**
 * Sorts an array of integers. Speed is configurable.
 * @param {array} arr The array to sorted
 * @param {number} speed How fast to sort the array
 * @returns A promise that will be fulfilled with the sorted array
 */
async function sleepsort(arr, speed = 0) {
    const max_n = arr.reduce((a,b) => Math.max(a,b));
    let time_speedup = max_n * speed;
    if (speed <= 1) {
        time_speedup = 1;
    }

    return new Promise((fulfill) => {
        sorted = [];
        for (let n of arr) {
            if (n === max_n) {
                setTimeout(() => {
                    sorted.push(n);
                    fulfill(sorted);
                }, n / time_speedup);
            } else {
                setTimeout(() => sorted.push(n), n / time_speedup);
            }
        }
    })
}

const arr = [20, 5, 100, 1, 90, 200, 40, 29];
sleepsort(arr).then(sorted => console.log(sorted))
