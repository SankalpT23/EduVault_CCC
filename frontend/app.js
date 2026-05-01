

const STORAGE_KEY = 'eduvault_students';

function loadStudents() {
    try {
        const data = localStorage.getItem(STORAGE_KEY);
        return data ? JSON.parse(data) : [];
    } catch {
        return [];
    }
}

function saveStudents(students) {
    localStorage.setItem(STORAGE_KEY, JSON.stringify(students));
}


function calculateBadge(cgpa, attendance) {
    if (cgpa >= 9.0 && attendance > 90) return 'ELITE SCHOLAR';
    if (cgpa >= 8.0) return 'RISING STAR';
    if (attendance < 75) return 'RISK ALERT';
    return 'REGULAR';
}

function generateAISkill(branch, cgpa) {
    switch (branch) {
        case 'CSE':   return cgpa > 8.5 ? 'AI/ML Architect' : 'Web Developer';
        case 'ECE':   return cgpa > 8.5 ? 'Embedded Designer' : 'IoT Specialist';
        case 'ME':    return cgpa > 8.5 ? 'Robotics Eng.' : 'CAD Designer';
        case 'EEE':   return cgpa > 8.5 ? 'Power Systems' : 'Circuit Designer';
        default:      return 'Data Analyst';
    }
}

function getMeritScore(cgpa, attendance) {
    return Math.floor(cgpa * 10 + attendance * 0.5);
}

function getScholarshipCost(branch) {
    if (branch === 'CSE' || branch === 'ECE') return 50;
    if (branch === 'ME' || branch === 'EEE') return 40;
    return 30;
}

function badgeHTML(badge) {
    const cls = {
        'ELITE SCHOLAR': 'badge-elite',
        'RISING STAR': 'badge-rising',
        'RISK ALERT': 'badge-alert',
        'REGULAR': 'badge-regular'
    }[badge] || 'badge-regular';
    return `<span class="badge ${cls}">${badge}</span>`;
}

function attHTML(att) {
    return `<span class="${att >= 75 ? 'att-ok' : 'att-low'}">${att}%</span>`;
}


function mergeSort(arr, compareFn) {
    if (arr.length <= 1) return arr;
    const mid = Math.floor(arr.length / 2);
    const left  = mergeSort(arr.slice(0, mid), compareFn);
    const right = mergeSort(arr.slice(mid), compareFn);
    return merge(left, right, compareFn);
}

function merge(left, right, compareFn) {
    const result = [];
    let i = 0, j = 0;
    while (i < left.length && j < right.length) {
        if (compareFn(left[i], right[j]) <= 0) {
            result.push(left[i++]);
        } else {
            result.push(right[j++]);
        }
    }
    while (i < left.length) result.push(left[i++]);
    while (j < right.length) result.push(right[j++]);
    return result;
}


function binarySearch(sortedArr, targetId) {
    let lo = 0, hi = sortedArr.length - 1;
    let steps = 0;
    while (lo <= hi) {
        steps++;
        const mid = Math.floor((lo + hi) / 2);
        if (sortedArr[mid].id === targetId) return { index: mid, steps };
        if (sortedArr[mid].id < targetId)   lo = mid + 1;
        else                                 hi = mid - 1;
    }
    return { index: -1, steps };
}


function knapsack01(students, budget) {
    const n = students.length;
    const W = budget;

    const dp = Array.from({ length: n + 1 }, () => new Array(W + 1).fill(0));

    for (let i = 1; i <= n; i++) {
        const cost  = students[i - 1].scholarshipCost;
        const merit = students[i - 1].meritScore;
        for (let w = 0; w <= W; w++) {
            dp[i][w] = dp[i - 1][w]; // don't pick
            if (cost <= w) {
                dp[i][w] = Math.max(dp[i][w], dp[i - 1][w - cost] + merit); // pick
            }
        }
    }

    const selected = [];
    let w = W;
    for (let i = n; i >= 1; i--) {
        if (dp[i][w] !== dp[i - 1][w]) {
            selected.push(i - 1);
            w -= students[i - 1].scholarshipCost;
        }
    }

    return {
        maxMerit: dp[n][W],
        selected: selected.reverse(),
        budgetUsed: budget - w,
        tableSize: (n + 1) * (W + 1)
    };
}


const navLinks = document.querySelectorAll('.nav-link');
const pages    = document.querySelectorAll('.page');

navLinks.forEach(link => {
    link.addEventListener('click', (e) => {
        e.preventDefault();
        const target = link.dataset.page;

        navLinks.forEach(l => l.classList.remove('active'));
        link.classList.add('active');

        pages.forEach(p => p.classList.remove('active'));
        document.getElementById(`page-${target}`).classList.add('active');

        if (target === 'dashboard') renderDashboard();
        if (target === 'students')  renderAllStudents();
    });
});


function renderDashboard() {
    const students = loadStudents();
    const total = students.length;

    document.getElementById('stat-total').textContent = total;

    if (total > 0) {
        const avgCgpa = (students.reduce((s, x) => s + x.cgpa, 0) / total).toFixed(2);
        const elites  = students.filter(s => s.badge === 'ELITE SCHOLAR').length;
        const alerts  = students.filter(s => s.attendance < 75).length;

        document.getElementById('stat-avg-cgpa').textContent = avgCgpa;
        document.getElementById('stat-elites').textContent   = elites;
        document.getElementById('stat-alerts').textContent   = alerts;
    } else {
        document.getElementById('stat-avg-cgpa').textContent = '0.00';
        document.getElementById('stat-elites').textContent   = '0';
        document.getElementById('stat-alerts').textContent   = '0';
    }

    const recent = students.slice(-5).reverse();
    const tbody  = document.getElementById('recent-tbody');

    if (recent.length === 0) {
        tbody.innerHTML = `<tr><td colspan="6"><div class="empty-state">
            <div class="empty-icon">📭</div><p>No students yet. Register one to get started!</p></div></td></tr>`;
        return;
    }

    tbody.innerHTML = recent.map(s => `
        <tr>
            <td>${s.id}</td>
            <td>${s.name}</td>
            <td>${s.branch}</td>
            <td>${s.cgpa}</td>
            <td>${attHTML(s.attendance)}</td>
            <td>${badgeHTML(s.badge)}</td>
        </tr>
    `).join('');
}


document.getElementById('register-form').addEventListener('submit', (e) => {
    e.preventDefault();
    const resultDiv = document.getElementById('register-result');
    resultDiv.classList.remove('hidden', 'success', 'error', 'info');

    const id   = parseInt(document.getElementById('reg-id').value);
    const name = document.getElementById('reg-name').value.trim();
    const branch = document.getElementById('reg-branch').value;
    const cgpa = parseFloat(document.getElementById('reg-cgpa').value);
    const att  = parseFloat(document.getElementById('reg-att').value);

    if (!name) {
        resultDiv.classList.add('error');
        resultDiv.innerHTML = '❌ Please enter a valid name.';
        resultDiv.classList.remove('hidden');
        return;
    }

    const students = loadStudents();

    if (students.find(s => s.id === id)) {
        resultDiv.classList.add('error');
        resultDiv.innerHTML = `❌ Student with ID <strong>${id}</strong> already exists!`;
        resultDiv.classList.remove('hidden');
        return;
    }

    const badge   = calculateBadge(cgpa, att);
    const aiSkill = generateAISkill(branch, cgpa);
    const merit   = getMeritScore(cgpa, att);
    const cost    = getScholarshipCost(branch);

    const student = {
        id, name, branch, cgpa, attendance: att,
        badge, aiSkill, meritScore: merit, scholarshipCost: cost
    };

    students.push(student);
    saveStudents(students);

    resultDiv.classList.add('success');
    resultDiv.innerHTML = `
        ✅ <strong>${name}</strong> registered successfully!<br>
        <small>Badge: <strong>${badge}</strong> | AI Skill: <strong>${aiSkill}</strong> | Merit: <strong>${merit}</strong></small>
    `;
    resultDiv.classList.remove('hidden');

    document.getElementById('register-form').reset();
});


function renderAllStudents() {
    const students = loadStudents();
    const tbody = document.getElementById('all-tbody');
    const count = document.getElementById('all-count');

    if (students.length === 0) {
        tbody.innerHTML = `<tr><td colspan="9"><div class="empty-state">
            <div class="empty-icon">📭</div><p>No students in database.</p></div></td></tr>`;
        count.textContent = '';
        return;
    }

    tbody.innerHTML = students.map(s => `
        <tr>
            <td>${s.id}</td>
            <td>${s.name}</td>
            <td>${s.branch}</td>
            <td>${s.cgpa}</td>
            <td>${attHTML(s.attendance)}</td>
            <td>${badgeHTML(s.badge)}</td>
            <td>${s.aiSkill}</td>
            <td>${s.meritScore}</td>
            <td><button class="btn btn-danger" onclick="deleteStudent(${s.id})">Delete</button></td>
        </tr>
    `).join('');

    count.textContent = `Total: ${students.length} student(s)`;
}

function deleteStudent(id) {
    if (!confirm(`Delete student ID ${id}?`)) return;
    let students = loadStudents();
    students = students.filter(s => s.id !== id);
    saveStudents(students);
    renderAllStudents();
}


document.getElementById('btn-search').addEventListener('click', () => {
    const resultDiv = document.getElementById('search-result');
    const stepsDiv  = document.getElementById('search-steps');
    resultDiv.classList.add('hidden');
    stepsDiv.classList.add('hidden');
    resultDiv.classList.remove('success', 'error', 'info');

    const targetId = parseInt(document.getElementById('search-id').value);
    if (isNaN(targetId) || targetId < 1) {
        resultDiv.classList.remove('hidden');
        resultDiv.classList.add('error');
        resultDiv.innerHTML = '❌ Enter a valid Student ID.';
        return;
    }

    let students = loadStudents();
    const n = students.length;

    if (n === 0) {
        resultDiv.classList.remove('hidden');
        resultDiv.classList.add('error');
        resultDiv.innerHTML = '❌ Database is empty.';
        return;
    }

    students = mergeSort(students, (a, b) => a.id - b.id);

    const { index, steps } = binarySearch(students, targetId);

    if (index !== -1) {
        const s = students[index];
        resultDiv.classList.remove('hidden');
        resultDiv.classList.add('success');
        resultDiv.innerHTML = `
            ✅ <strong>Student Found!</strong>
            <div class="profile-card">
                <div class="profile-item"><span class="p-label">ID</span><span class="p-value">${s.id}</span></div>
                <div class="profile-item"><span class="p-label">Name</span><span class="p-value">${s.name}</span></div>
                <div class="profile-item"><span class="p-label">Branch</span><span class="p-value">${s.branch}</span></div>
                <div class="profile-item"><span class="p-label">CGPA</span><span class="p-value">${s.cgpa}</span></div>
                <div class="profile-item"><span class="p-label">Attendance</span><span class="p-value">${s.attendance}%</span></div>
                <div class="profile-item"><span class="p-label">Badge</span><span class="p-value">${s.badge}</span></div>
                <div class="profile-item"><span class="p-label">AI Skill</span><span class="p-value">${s.aiSkill}</span></div>
                <div class="profile-item"><span class="p-label">Merit Score</span><span class="p-value">${s.meritScore}</span></div>
            </div>`;
    } else {
        resultDiv.classList.remove('hidden');
        resultDiv.classList.add('error');
        resultDiv.innerHTML = `❌ Student with ID <strong>${targetId}</strong> not found.`;
    }

    stepsDiv.classList.remove('hidden');
    stepsDiv.innerHTML = `Algorithm: Binary Search | Steps taken: ${steps} | Max possible: ~${Math.ceil(Math.log2(n + 1))} (log₂${n}) | Linear scan would take: ${n} steps`;
});


document.getElementById('btn-sort').addEventListener('click', () => {
    const criteria = document.getElementById('sort-criteria').value;
    let students = loadStudents();
    const infoDiv = document.getElementById('sort-info');

    if (students.length === 0) {
        document.getElementById('rank-tbody').innerHTML = `<tr><td colspan="8"><div class="empty-state">
            <div class="empty-icon">📭</div><p>No students to rank.</p></div></td></tr>`;
        infoDiv.classList.add('hidden');
        return;
    }

    let compareFn;
    let label;
    switch (criteria) {
        case 'cgpa':
            compareFn = (a, b) => b.cgpa - a.cgpa;
            label = 'CGPA (Highest First)';
            break;
        case 'attendance':
            compareFn = (a, b) => b.attendance - a.attendance;
            label = 'Attendance (Highest First)';
            break;
        case 'name':
            compareFn = (a, b) => a.name.localeCompare(b.name);
            label = 'Name (A → Z)';
            break;
        case 'merit':
            compareFn = (a, b) => b.meritScore - a.meritScore;
            label = 'Merit Score (Highest First)';
            break;
    }

    const sorted = mergeSort(students, compareFn);

    const tbody = document.getElementById('rank-tbody');
    tbody.innerHTML = sorted.map((s, i) => {
        let rankClass = '';
        let rankText  = `#${i + 1}`;
        if (i === 0)      { rankClass = 'rank-gold';   rankText = '🥇 #1'; }
        else if (i === 1) { rankClass = 'rank-silver'; rankText = '🥈 #2'; }
        else if (i === 2) { rankClass = 'rank-bronze'; rankText = '🥉 #3'; }
        return `
            <tr>
                <td class="${rankClass}">${rankText}</td>
                <td>${s.id}</td>
                <td>${s.name}</td>
                <td>${s.branch}</td>
                <td>${s.cgpa}</td>
                <td>${attHTML(s.attendance)}</td>
                <td>${badgeHTML(s.badge)}</td>
                <td>${s.meritScore}</td>
            </tr>`;
    }).join('');

    infoDiv.classList.remove('hidden');
    infoDiv.innerHTML = `Algorithm: Merge Sort | Complexity: O(n log n) | Sorted by: ${label} | n = ${sorted.length}`;
});


document.getElementById('btn-knapsack').addEventListener('click', () => {
    const budget = parseInt(document.getElementById('budget-input').value);
    const resultDiv = document.getElementById('knapsack-result');
    resultDiv.classList.add('hidden');

    if (isNaN(budget) || budget < 1) {
        alert('Enter a valid budget (in thousands).');
        return;
    }

    const students = loadStudents();
    if (students.length === 0) {
        alert('No students in database.');
        return;
    }

    if (budget > 10000) {
        if (!confirm(`Budget of ${budget}K will create a large DP table. Continue?`)) return;
    }

    const result = knapsack01(students, budget);

    const statsDiv = document.getElementById('knapsack-stats');
    statsDiv.innerHTML = `
        <div class="ks-stat"><div class="ks-val" style="color:#10b981">${result.selected.length}/${students.length}</div><div class="ks-label">Students Selected</div></div>
        <div class="ks-stat"><div class="ks-val" style="color:#6366f1">${result.maxMerit}</div><div class="ks-label">Total Merit</div></div>
        <div class="ks-stat"><div class="ks-val" style="color:#f59e0b">${result.budgetUsed}K / ${budget}K</div><div class="ks-label">Budget Used</div></div>
        <div class="ks-stat"><div class="ks-val" style="color:#06b6d4">${budget - result.budgetUsed}K</div><div class="ks-label">Budget Remaining</div></div>
    `;

    const tbody = document.getElementById('knapsack-tbody');
    if (result.selected.length === 0) {
        tbody.innerHTML = `<tr><td colspan="5"><div class="empty-state"><p>Budget too low to award any scholarship.</p></div></td></tr>`;
    } else {
        tbody.innerHTML = result.selected.map(idx => {
            const s = students[idx];
            return `<tr class="row-selected"><td>${s.id}</td><td>${s.name}</td><td>${s.branch}</td><td>${s.meritScore}</td><td>${s.scholarshipCost}K</td></tr>`;
        }).join('');
    }

    document.getElementById('knapsack-algo-info').innerHTML =
        `Algorithm: 0/1 Knapsack DP | Time: O(n×W) | n=${students.length}, W=${budget} | DP Table: ${result.tableSize.toLocaleString()} cells`;

    resultDiv.classList.remove('hidden');
});


renderDashboard();
