'use client'
import { BACKEND_ADDR } from "@/next.config";
import { useState } from "react";
import Cookies from "js-cookie";

export default function upload(){
	const [filename, setFilename] = useState("");
	const [filedata, setFiledata] = useState("");

	const handleSubmit = async () => {
		const response = await fetch(`${BACKEND_ADDR}/api/v1/update_file?type=csv&file=${filename}.csv`, {
			method: 'POST',
			headers: {
				"Content-Type": "application/json",
				'X-Client-Email': String(Cookies.get('email')),
				'Authorization': String(Cookies.get('token'))
			},
			body: JSON.stringify(filedata)
		});

		if (response.ok) {
			console.log('Successfully submitted');
		} else {
			console.log(response);
			console.log('Submission failed...?');
		}
	}

	return (
		<div className="w-full h-full">
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

			<textarea
				value={filedata}
				onChange={e => setFiledata(e.target.value)}
				className="border px-2 py-1 mr-2 text-black bg-white w-7xl h-100">
			</textarea>

			<button
				type="submit"
				className="mt-4 bg-green-600 text-white px-4 py-2 rounded hover:bg-green-700"
				onClick={handleSubmit}
			>
				Submit File
			</button>
		</div>
	)
}