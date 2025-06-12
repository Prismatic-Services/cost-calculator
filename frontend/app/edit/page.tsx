'use client';

import { useState } from "react";
import { BACKEND_ADDR } from "@/next.config";
import Cookies from "js-cookie";
import { assigning_keys, fetch_file, format_as_array } from './fetch_data';
import { ToastMessage } from "@/app/ToastMessage";

type status_code = "ok" | "error" | "warning" | "info";

export default function Edit() {
	const [filename, setFilename] = useState('');
	const [dataMap, setDataMap] = useState<Map<string, string> | null>(null);
	const [loading, setLoading] = useState(false);
	const [submitted, setSubmitted] = useState(false);
	const [toast, setToast] = useState<{ status: status_code; message: string } | null>(null);

	const showToast = (status: status_code, message: string) => {
		setToast({ status, message });
	};

	const getData = async (filename: string) => {
		setLoading(true);
		const raw = await fetch_file(filename);

		if (!raw) {
			showToast("error", "Failed to load file.");
			setLoading(false);
			return;
		}

		try {
			const formatted = format_as_array(raw);
			const keyed = assigning_keys(formatted);
			setDataMap(keyed);
			showToast("ok", "File loaded successfully.");
		} catch (err: any) {
			console.error(err);
			showToast("error", "Error parsing file data.");
		}

		setLoading(false);
		setSubmitted(false);
	};

	const handleChange = (key: string, value: string) => {
		if (!dataMap) return;
		const newMap = new Map(dataMap);
		newMap.set(key, value);
		setDataMap(newMap);
	};

	const handleSubmit = async () => {
		if (!dataMap) return;

		const response = await fetch(`${BACKEND_ADDR}/api/v1/update_file?file=${filename}.csv`, {
			method: 'POST',
			headers: {
				"Content-Type": "application/json",
				'X-Client-Email': String(Cookies.get('email')),
				'Authorization': String(Cookies.get('token'))
			},
			body: JSON.stringify(Object.fromEntries(dataMap))
		});

		if (response.ok) {
			setSubmitted(true);
			showToast("ok", "Changes submitted successfully.");
		} else {
			const err = await response.json();
			showToast("error", err.message || "Submission failed.");
		}
	};

	const groupByLeadingNumber = (map: Map<string, string>) => {
		const groups: { [key: string]: [string, string][] } = {};

		for (const [key, value] of map.entries()) {
			const match = key.match(/^\d+/);
			const groupKey = match ? match[0] : 'other';

			if (!groups[groupKey]) groups[groupKey] = [];
			groups[groupKey].push([key, value]);
		}

		return Object.fromEntries(
			Object.entries(groups).sort(([a], [b]) => parseInt(a) - parseInt(b))
		);
	};

	return (
		<div className="p-4 relative">
			<div className="mb-4">
				<select
					value={filename}
					onChange={e => setFilename(e.target.value)}
					className="border px-2 py-1 mr-2 text-black bg-white"
				>
					<option value="">Select a file</option>
					<option value="Buses and Drivers">Buses and Drivers.csv</option>
					<option value="Daily Bus Routes">Daily Bus Routes.csv</option>
					<option value="Driver Payment Scale">Driver Payment Scale.csv</option>
					<option value="Maintenance Payment Scale">Maintenance Payment Scale.csv</option>
					<option value="Management Payment Scale">Management Payment Scale.csv</option>
					<option value="PVA Table">PVA Table.csv</option>
				</select>

				<button
					onClick={() => getData(filename)}
					disabled={!filename}
					className="bg-yellow-600 text-white px-4 py-1 rounded hover:bg-yellow-700 disabled:opacity-50"
				>
					Load
				</button>
			</div>

			{loading && <p>Loading...</p>}

			{dataMap && (
				<form
					onSubmit={(e) => {
						e.preventDefault();
						handleSubmit();
					}}
					className="space-y-4"
				>
					{Object.entries(groupByLeadingNumber(dataMap)).map(([groupKey, fields]) => (
						<div key={groupKey} className="flex gap-4 flex-wrap items-center">
							{fields.map(([key, value]) => (
								<div key={key} className="flex flex-col items-start">
									<label className="text-sm font-medium">{key}</label>
									<input
										type="text"
										value={value}
										placeholder="Empty Cell"
										onChange={(e) => handleChange(key, e.target.value)}
										className="border px-2 py-1 rounded w-40"
									/>
								</div>
							))}
						</div>
					))}

					<button
						type="submit"
						className="mt-4 bg-green-600 text-white px-4 py-2 rounded hover:bg-green-700"
					>
						Submit Changes
					</button>
				</form>
			)}

			{submitted && <p className="text-green-700 mt-4">Submitted successfully!</p>}

			{toast && (
				<ToastMessage
					status={toast.status}
					message={toast.message}
					onClose={() => setToast(null)}
				/>
			)}
		</div>
	);
}
